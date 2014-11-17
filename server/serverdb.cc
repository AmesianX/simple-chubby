
#include <assert.h>
#include <stdint.h>

#include <unistd.h>

#include <set>
#include <string>
#include <iostream>
#include <memory>

#include "sqlite3.h"

#include "sqlstmt.h"
#include "serverdb.h"

using namespace std;

int
dirDepth(const string &path)
{
    int depth = 0;

    for (auto it = path.begin(); it != path.end(); it++) {
        if ((*it) == '/')
            depth++;
    }

    return depth;
}

ServerDB::ServerDB(const char *file)
{
    open(file);
}

ServerDB::~ServerDB()
{
    close();
}

bool
ServerDB::hasKey(const string &path)
{
    SQLStmt s(db, "SELECT key, value FROM kvpair WHERE key = \"%s\"",
            path.c_str());

    return s.step().row();
}


bool
ServerDB::hasName(const string &path)
{
    SQLStmt s(db, "SELECT name FROM fs WHERE name = \"%s\"",
            path.c_str());

    return s.step().row();
}

string
ServerDB::get(const string &path)
{
    SQLStmt s(db, "SELECT value FROM kvpair WHERE key = \"%s\"",
            path.c_str());

    s.step();
    if (!s.row()) {
        cout << "No Key: " << path << endl;
        throw runtime_error("Key not present");
    }

    return s.str(0);
}

bool
ServerDB::set(const string &path, const string &val)
{
    bool update = hasKey(path);
    sqlexec("BEGIN;");
    if (update) {
        sqlexec("UPDATE kvpair SET value = \"%s\" WHERE key = \"%s\";",
            val.c_str(), path.c_str());
    } else {
        int depth = dirDepth(path);
        sqlexec("INSERT INTO kvpair (key, value, depth) VALUES (\"%s\", \"%s\", %d);",
            path.c_str(), val.c_str(), depth);
    }
    sqlexec("COMMIT;");
    return update;
}

void
ServerDB::remove(const string &path)
{
    if (!hasKey(path))
        throw runtime_error("Key not found");

    sqlexec("BEGIN;");
    sqlexec("DELETE FROM kvpair WHERE key = \"%s\";", path.c_str());
    sqlexec("COMMIT;");
}

set<string>
ServerDB::list(const string &path)
{
    int depth = dirDepth(path) + 1;
    std::set<string> r;
    SQLStmt s(db, "SELECT key FROM kvpair WHERE key LIKE '%s/%%' AND depth = %d",
            path.c_str(), depth);

    s.step();
    while (s.row()) {
        r.insert(s.str(0));
        s.step();
    }

    return r;
}

bool
ServerDB::checkAndCreate(const std::string &file_name, bool is_dir, uint64_t instance_number)
{
    string parent_name = getParentName(file_name);
    bool file_exists = hasName(file_name);
    bool parent_exists = parent_name.compare("/") == 0 ||  // root dir ('/') always exists
	hasName(parent_name);
    
    if (file_exists || !parent_exists)
	return false;
    // TODO check parent is directory

    // begins to create the file
    sqlexec("BEGIN;");
    sqlexec("INSERT INTO fs (name, instance_number, is_directory) VALUES (\"%s\", %d, %d);",
            file_name.c_str(), instance_number, is_dir);
    // TODO update the content field of parent dir
    sqlexec("COMMIT;");


    return true;
}

bool
ServerDB::checkAndOpen(const std::string &file_name, uint64_t *instance_number)
{
    bool file_exists = hasName(file_name);
    if (!file_exists)
	return false;

    SQLStmt s(db, "SELECT instance_number FROM fs WHERE name = \"%s\"",
            file_name.c_str());

    s.step();
    if (!s.row()) {
        cout << "No Key: " << file_name << endl;
        throw runtime_error("Key not present");
    }
    *instance_number = s.integer(0);

    return true;
}


void
ServerDB::create(const char *file)
{
    int status;

    status = sqlite3_open_v2(file,
                             &db,
                             SQLITE_OPEN_READWRITE|SQLITE_OPEN_CREATE,
                             NULL);
    if (status != SQLITE_OK) {
        cerr << sqlite3_errmsg(db) << endl;
        assert(false);
    }
    string table_params = string("(") + 
	"name TEXT PRIMARY KEY NOT NULL, " +
	"content TEXT, " +
	"lock_owner INT, " +
	"instance_number INT, " +
	"content_generation_number INT DEFAULT 0, " +
	"lock_generation_number INT DEFAULT 0, " +
	"file_content_checksum INT, " +
	"is_directory INT " +
	");";
    sqlexec("BEGIN;");
    sqlexec((string("CREATE TABLE fs ") + table_params).c_str());
    sqlexec("COMMIT;");
}

void
ServerDB::open(const char *file)
{
    int status;

    status = access(file, F_OK);
    if (status < 0 && errno == ENOENT) {
        create(file);
        return;
    }

    status = sqlite3_open_v2(file,
                             &db,
                             SQLITE_OPEN_READWRITE,
                             NULL);
    if (status != SQLITE_OK) {
        cerr << sqlite3_errmsg(db) << endl;
        assert(false);
    }
}

void
ServerDB::close()
{
    int status = sqlite3_close(db);
    if (status != SQLITE_OK) {
        cerr << sqlite3_errmsg(db) << endl;
        assert(false);
    }
}

void
ServerDB::sqlexec(const char *fmt, ...)
{
    char *query;
    va_list ap;

    va_start (ap, fmt);
    query = sqlite3_vmprintf (fmt, ap);
    unique_ptr<char, decltype(&sqlite3_free)> _c(query, sqlite3_free);
    va_end (ap);

    if (!query)
        throw runtime_error("sqlite3_vmprintf: out of memory in sqlexec");

    int err = sqlite3_exec (db, query, NULL, NULL, NULL);
    if (err != SQLITE_OK && err != SQLITE_DONE && err != SQLITE_ROW) {
        cerr << sqlite3_errmsg(db) << endl;
        throw runtime_error("sqlite3_exec: ");
    }
}

string
ServerDB::getParentName(const string &key)
{
    // find the lenth of the parent of KEY
    int pos = key.length() - 1;
    while (pos > 0) { /* if pos==0, the parent is root, 
			 then break and return "/" */
	if (key[pos] == '/') break;
	--pos;
    }
    if (pos == 0)
	return "/";
    else 
	return string(key, 0, pos);  // create a substring of key
}

#if 0

int
main(int argc, const char *argv[])
{
    ServerDB s("test.db");
    uint64_t n = 0;
    bool r;

    r = s.checkAndCreate("/test", false, 1);
    cout << "create /test ";
    if(r)
	cout << "succeeded." << endl;
    else cout << "failed." << endl;

    r = s.checkAndCreate("/test", false, 2);
    cout << "create /test ";
    if(r)
	cout << "succeeded." << endl;
    else cout << "failed." << endl;

    r = s.checkAndOpen("/test", &n);
    cout << "open /test ";
    if(r)
	cout << "succeeded: instance_number = "<< n << endl;
    else cout << "failed." << endl;

    r = s.checkAndOpen("/test/a", &n);
    cout << "open /test/a ";
    if(r)
	cout << "succeeded: instance_number = "<< n << endl;
    else cout << "failed." << endl;
/*
    s.set("/test", "myval");
    s.get("/test");
    s.set("/test", "newval");
    s.get("/test");
    s.set("/test/a", "");
    s.set("/test/b", "");
    set<string> lst = s.list("/test");
    for (auto it = lst.begin(); it != lst.end(); it++) {
        cout << "LIST: " << (*it) << endl;
    }
    if (s.hasKey("/bob"))
        cout << "Good 'bob' not present" << endl;
*/
}


#endif
