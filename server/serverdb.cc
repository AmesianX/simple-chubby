
#include <assert.h>
#include <stdint.h>
#include <errno.h>

#include <unistd.h>

#include <set>
#include <string>
#include <iostream>
#include <memory>

#include "sqlite3.h"

#include "sqlstmt.h"
#include "serverdb.h"

using namespace std;


ServerDB::ServerDB(const char *file)
{
    open(file);
}

ServerDB::~ServerDB()
{
    close();
}



bool
ServerDB::hasName(const string &path)
{
    SQLStmt s(db, "SELECT name FROM fs WHERE name = \"%s\"",
            path.c_str());

    return s.step().row();
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


bool 
ServerDB::checkAndDelete(const std::string &file_name, uint64_t instance_number)
{
    SQLStmt s(db, "SELECT lock_owner, instance_number, is_directory FROM fs WHERE name = \"%s\"",
            file_name.c_str());

    s.step();
    if (!s.row()) {
	// node does not exist
	return false;
    }

    // check lock is held
    uint64_t lock_owner = s.integer(0);
    if (lock_owner != 0)
	return false;

    // check INSTANCE_NUMBER
    uint64_t node_instance_number = s.integer(1);
    if (node_instance_number != instance_number)
	return false;

    // check dir is empty
    bool is_dir = s.integer(2);
    if (is_dir){
	// query for its children
	SQLStmt s(db, "SELECT name FROM fs WHERE name LIKE '%s/%%'",
		  file_name.c_str());
	s.step();
	if (s.row()) // if returned non-empty result
	    return false;
    }

    // delete 
    sqlexec("BEGIN;");
    sqlexec("DELETE FROM fs WHERE name = \"%s\";", file_name.c_str());
    sqlexec("COMMIT;");
    
    return true;
}
bool
ServerDB::checkAndRead(const std::string &file_name, uint64_t instance_number,
		       std::string *content, MetaData *meta)
{
    SQLStmt s(db, "SELECT content, instance_number, content_generation_number, lock_generation_number, file_content_checksum, is_directory FROM fs WHERE name = \"%s\"",
	      file_name.c_str());

    s.step();
    if (!s.row()) {
	// node does not exist
	return false;
    }

    // check INSTANCE_NUMBER
    uint64_t node_instance_number = s.integer(1);
    if (node_instance_number != instance_number)
	return false;

    *content = s.str(0);
    meta->instance_number = node_instance_number;
    meta->content_generation_number = s.integer(2);
    meta->lock_generation_number = s.integer(3);
    meta->file_content_checksum = s.integer(4);
    meta->is_directory = s.integer(5);

    return true;
}

bool
ServerDB::checkAndUpdate(const std::string &file_name, uint64_t instance_number,
			 const std::string &content)
{
    SQLStmt s(db, "SELECT instance_number, content_generation_number FROM fs WHERE name = \"%s\"",
	      file_name.c_str());

    s.step();
    if (!s.row()) {
	// node does not exist
	return false;
    }

    // check INSTANCE_NUMBER
    uint64_t node_instance_number = s.integer(0);
    if (node_instance_number != instance_number)
	return false;
    
    uint64_t new_content_generation_number = s.integer(1) + 1;
    
    std::hash<std::string> str_hash;
    uint64_t new_checksum = str_hash(content);

    // do update
    sqlexec("BEGIN;");
    sqlexec("UPDATE fs SET content = \"%s\", content_generation_number = %d, file_content_checksum = %d  WHERE name = \"%s\";",
            content.c_str(), new_content_generation_number, new_checksum, file_name.c_str());
    sqlexec("COMMIT;");

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
	"lock_owner INT DEFAULT 0, " +
	"instance_number INT, " +
	"content_generation_number INT DEFAULT 0, " +
	"lock_generation_number INT DEFAULT 0, " +
	"file_content_checksum INT DEFAULT 0, " +
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

#if 1

int
main(int argc, const char *argv[])
{
    ServerDB s("test.db");
    uint64_t n = 0;
    bool r;

    r = s.checkAndCreate("/test", false, 1);
    cout << "create /test (ver 1)";
    if(r)
	cout << "succeeded." << endl;
    else cout << "failed." << endl;

    r = s.checkAndCreate("/test", false, 2);
    cout << "create /test (ver 2)";
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


    r = s.checkAndDelete("/test/a", 1);
    cout << "delete /test/a ";
    if(r)
	cout << "succeeded." << endl;
    else cout << "failed." << endl;


    r = s.checkAndDelete("/test", 0);
    cout << "delete /test (ver 0)";
    if(r)
	cout << "succeeded." << endl;
    else cout << "failed." << endl;

    r = s.checkAndDelete("/test", 1);
    cout << "delete /test (ver 1)";
    if(r)
	cout << "succeeded." << endl;
    else cout << "failed." << endl;

    r = s.checkAndCreate("/test", false, 3);
    cout << "create /test ";
    if(r)
	cout << "succeeded." << endl;
    else cout << "failed." << endl;

    r = s.checkAndOpen("/test", &n);
    cout << "open /test ";
    if(r)
	cout << "succeeded: instance_number = "<< n << endl;
    else cout << "failed." << endl;

    std::string content;
    MetaData meta;
    r = s.checkAndRead("/test", 3, &content, &meta);
    cout << "read /test ";
    if(r) {
	cout << "succeeded." << endl;
	cout << content<< " "<< meta.instance_number << " "<< meta.content_generation_number<<
	    " "<< meta.lock_generation_number << " "<< meta.file_content_checksum << 
	    " "<< meta.is_directory<<endl;
    }
    else cout << "failed." << endl;


    r = s.checkAndUpdate("/test", 3, "Hello.");
    cout << "write /test ";
    if(r)
	cout << "succeeded."<< endl;
    else cout << "failed." << endl;
    
    r = s.checkAndRead("/test", 3, &content, &meta);
    cout << "read /test ";
    if(r) {
	cout << "succeeded." << endl;
	cout << content<< " "<< meta.instance_number << " "<< meta.content_generation_number<<
	    " "<< meta.lock_generation_number << " "<< meta.file_content_checksum << 
	    " "<< meta.is_directory<<endl;
    }
    else cout << "failed." << endl;
    
}


#endif
