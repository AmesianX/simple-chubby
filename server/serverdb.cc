
#include <assert.h>
#include <stdint.h>
#include <errno.h>
#include <unistd.h>

#include <string>
#include <iostream>
#include <sstream>
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

inline string
dirContentCreate(const string &content, const string &node)
{
  return string(content + "|" + node);
}


inline string
dirContentDelete(const string &content, const string &node)
{
  std::string str;
  std::stringstream ss(content);
  /* split CONTENT by a delim '|', append tokens to a new string STR
     except token equal to NODE. */
  std::string token;
  char delim = '|';
  while (std::getline(ss, token, delim)) {
    if(token.empty())
      continue;
    if(node.compare(token) != 0)
      str.append(std::string("|") + token);
  }
  return str;
}


bool
ServerDB::checkAndCreate(const std::string &file_name, bool is_dir, uint64_t *instance_number)
{
  cout<<"db: checkAndCreate("<<file_name<<", "<< is_dir<<") ";
  // TODO implement instance_number
  // check if node exits
  SQLStmt node_q(db, "SELECT name FROM fs WHERE name = \"%s\"", file_name.c_str());
  if(node_q.step().row()) {
    cout << " fails (node exits)."<<endl;
    return false;
  }

  // check if parent node is an existing directory
  std::string parent_name = getParentName(file_name);
  std::string parent_content;
  uint64_t parent_cn;
  if (parent_name.compare("/") != 0) { // root dir ('/') always exists
    SQLStmt par_q(db, "SELECT content, content_generation_number, is_directory "
		  "FROM fs WHERE name = \"%s\"",
		  parent_name.c_str());
    par_q.step();
    if(!par_q.row()) {
      cout << "fails (parent node doesn't exit)."<<endl;
      return false;
    }
    if(par_q.integer(2) == 0) {
      cout << "fails (parent node is not a directory)."<<endl;
      return false;
    }
    // get the directory content and content number
    parent_content = par_q.str(0);
    parent_cn = par_q.integer(1);
  }

  // get instance_number in db
  SQLStmt kv_q(db, "SELECT value FROM kvpairs WHERE key = \"max_instance_number\"");
  kv_q.step();
  assert(kv_q.row());
  uint64_t max_instance_num = kv_q.integer(0);
  ++max_instance_num;

  // begins to create the file
  sqlexec("BEGIN;");
  // insert node
  sqlexec("INSERT INTO fs (name, instance_number, is_directory) "
	  "VALUES (\"%s\", %d, %d);",
	  file_name.c_str(), max_instance_num, is_dir);
  // update max_instance_number
  sqlexec("UPDATE kvpairs SET value = %d WHERE name = \"max_instance_number\";",
	  max_instance_num);

  // update the content field of parent dir
  if (parent_name.compare("/") != 0) {  // we don't maintain root node
    string new_parent_c = dirContentCreate(parent_content, file_name);
    uint64_t new_parent_cn = parent_cn + 1;
    uint64_t new_checksum = str_hash(new_parent_c);
    
    sqlexec("UPDATE fs SET content = \"%s\", "
	    "content_generation_number = %d, "
	    "file_content_checksum = %d  WHERE name = \"%s\";",
	    new_parent_c.c_str(), new_parent_cn,
	    new_checksum, parent_name.c_str());
  }
  sqlexec("COMMIT;");

  *instance_number =  max_instance_num;
  cout<< " succeeds. instance_number="<< *instance_number <<endl;


  return true;
}

bool
ServerDB::checkAndOpen(const std::string &file_name, uint64_t *instance_number)
{
  cout<<"db: checkAndOpen("<<file_name<<") ";

  // check if node exits
  SQLStmt s(db, "SELECT instance_number FROM fs WHERE name = \"%s\"",
	    file_name.c_str());
  s.step();
  if(!s.row()) {
    cout << " fails (node does not exit)."<<endl;
    return false;
  }
  
  *instance_number = s.integer(0);
  cout<< " succeeds."<<endl;

  return true;
}


bool 
ServerDB::checkAndDelete(const std::string &file_name, uint64_t instance_number)
{
  cout<<"db: checkAndDelete("<<file_name<<", "<<instance_number<<") ";
  SQLStmt s(db, "SELECT lock_owner, instance_number, is_directory, content "
	    "FROM fs WHERE name = \"%s\"",
            file_name.c_str());

  s.step();
  if (!s.row()) {
    // node does not exist
    cout<< " fails. node does not exist"<<endl;
    return false;
  }

  // check lock is held
  std::string lock_owner = s.str(0);
  if (lock_owner.compare(NO_OWNER) != 0) {
    cout<< " fails. lock is held"<<endl;
    return false;
  }

  // check INSTANCE_NUMBER
  uint64_t node_instance_number = s.integer(1);
  if (node_instance_number != instance_number){
    cout<< " fails. instance_number doesn't match"<<endl;
    return false;
  }

  // check if dir is empty
  bool is_dir = s.integer(2);
  if (is_dir){
    std::string content = s.str(3);
    if(content.compare("") != 0) {
      cout<< " fails. dir is not empty"<<endl;
      return false;
    }
  }
  
  // query the content of parent dir
  std::string parent_name = getParentName(file_name);
  std::string parent_content;
  uint64_t parent_cn;
  if (parent_name.compare("/") != 0) { // root dir ('/') always exists
    SQLStmt par_q(db, "SELECT content, content_generation_number, is_directory "
		  "FROM fs WHERE name = \"%s\"",
		  parent_name.c_str());
    par_q.step();
    assert(par_q.row());
    assert(par_q.integer(2) == 1);
    // get the directory content and content number
    parent_content = par_q.str(0);
    parent_cn = par_q.integer(1);
  }

  // execute delete
  sqlexec("BEGIN;");
  // delete node
  sqlexec("DELETE FROM fs WHERE name = \"%s\";", file_name.c_str());
  // update the content field of parent dir
  if (parent_name.compare("/") != 0) {  // we don't maintain root node
    string new_parent_c = dirContentDelete(parent_content, file_name);
    uint64_t new_parent_cn = parent_cn + 1;
    uint64_t new_checksum = str_hash(new_parent_c);
    
    sqlexec("UPDATE fs SET content = \"%s\", "
	    "content_generation_number = %d, "
	    "file_content_checksum = %d  WHERE name = \"%s\";",
	    new_parent_c.c_str(), new_parent_cn,
	    new_checksum, parent_name.c_str());
  }
  sqlexec("COMMIT;");
  cout<< " succeeds."<<endl;
    
  return true;
}


bool
ServerDB::checkAndRead(const std::string &file_name, uint64_t instance_number,
		       std::string *content, MetaData *meta)
{
  SQLStmt s(db, "SELECT content, instance_number, "
	    "content_generation_number, lock_generation_number, "
	    "file_content_checksum, is_directory FROM fs WHERE name = \"%s\"",
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
  SQLStmt s(db, "SELECT instance_number, content_generation_number "
	    "FROM fs WHERE name = \"%s\"",
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
  uint64_t new_checksum = str_hash(content);

  // do update
  sqlexec("BEGIN;");
  sqlexec("UPDATE fs SET content = \"%s\", "
	  "content_generation_number = %d, "
	  "file_content_checksum = %d  WHERE name = \"%s\";",
	  content.c_str(), new_content_generation_number,
	  new_checksum, file_name.c_str());
  sqlexec("COMMIT;");

  return true;
}

bool
ServerDB::testAndSetLockOwner(const std::string &file_name, uint64_t instance_number,
			      const std::string &client_id)
{
  cout<<"db: testAndSetLockOwner("<<file_name<<", "<<instance_number
      <<", "<< client_id<<") ";
  
  SQLStmt s(db, "SELECT instance_number, lock_owner, "
	    "lock_generation_number FROM fs WHERE name = \"%s\"",
	    file_name.c_str());

  s.step();
  if (!s.row()) {
    // node does not exist
    cout<< " fails. node does not exist"<<endl;
    return false;
  }

  // check INSTANCE_NUMBER
  uint64_t node_instance_number = s.integer(0);
  if (node_instance_number != instance_number) {
    cout<< " fails. instance_number doesn't match"<<endl;
    return false;
  }

  
  // check lock is held
  std::string lock_owner = s.str(1);
  if (lock_owner.compare(NO_OWNER) != 0) {
    cout<< " fails. lock is held"<<endl;
    return false;
  }
  
  uint64_t new_lock_generation_number = s.integer(2) + 1;

  // do update
  sqlexec("BEGIN;");
  sqlexec("UPDATE fs SET lock_owner = \"%s\", "
	  "lock_generation_number = %d WHERE name = \"%s\";",
	  client_id.c_str(), new_lock_generation_number, file_name.c_str());
  sqlexec("COMMIT;");
  cout<< " succeeds."<<endl;

  return true;
}

bool
ServerDB::resetLockOwner(const std::string &file_name, uint64_t instance_number)
{
  
  cout<<"db: resetLockOwner("<<file_name<<", "<<instance_number<<") ";
  
  SQLStmt s(db, "SELECT instance_number FROM fs WHERE name = \"%s\"",
	    file_name.c_str());

  s.step();
  if (!s.row()) {
    // node does not exist
    cout<< " fails. node does not exist"<<endl;
    return false;
  }

  // check INSTANCE_NUMBER
  uint64_t node_instance_number = s.integer(0);
  if (node_instance_number != instance_number) {
    cout<< " fails. lock is held"<<endl;
    return false;
  }
    
  // do update
  sqlexec("BEGIN;");
  sqlexec("UPDATE fs SET lock_owner = \"%s\" WHERE name = \"%s\";",
	  NO_OWNER.c_str(), file_name.c_str());
  sqlexec("COMMIT;");
  
  cout<< " succeeds."<<endl;
  return true;
}

void 
ServerDB::getStates
(std::unordered_map<std::string, std::unordered_set<std::string> > &client2heldLock)
{
  
  SQLStmt s(db, "SELECT name, lock_owner FROM fs");
  s.step();
  while(s.row()) {
    std::string node = s.str(0);
    std::string owner = s.str(1);
    client2heldLock[node].insert(owner);
    s.step();
  }
  return;
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
    "name TEXT PRIMARY KEY NOT NULL, "
    "content TEXT DEFAULT '', "
    "lock_owner TEXT DEFAULT \"" + NO_OWNER + "\", "
    "instance_number INT, "
    "content_generation_number INT DEFAULT 0, "
    "lock_generation_number INT DEFAULT 0, "
    "file_content_checksum INT DEFAULT 0, "
    "is_directory INT "
    ");";
  sqlexec("BEGIN;");
  sqlexec((string("CREATE TABLE fs ") + table_params).c_str());
  //create table for storing instance_number
  sqlexec("CREATE TABLE kvpairs"
	  "(key TEXT PRIMARY KEY NOT NULL, value INT);");
  sqlexec("INSERT INTO kvpairs (key, value) VALUES (\"max_instance_number\", 0);");

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


#if 0

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


  r = s.testAndSetLockOwner("/test", 3, "client_1");
  cout << "test_set_lock /test ";
  if(r)
    cout << "succeeded."<< endl;
  else cout << "failed." << endl;

  r = s.testAndSetLockOwner("/test", 3, "client_1");
  cout << "test_set_lock /test ";
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


  r = s.resetLockOwner("/test", 3);
  cout << "resetset_lock /test ";
  if(r)
    cout << "succeeded."<< endl;
  else cout << "failed." << endl;

  r = s.testAndSetLockOwner("/test", 3, "client_1");
  cout << "test_set_lock /test ";
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
