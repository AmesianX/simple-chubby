
#ifndef __SERVERDB_H__
#define __SERVERDB_H__

#include "sqlite3.h"

#include <set>
#include <string>
#include <cstdint>
#include "include/server.hh"

class ServerDB {
public:
  /*
   * Constructor for ServerDB
   *
   * Creates an instance backed by the given file name
   */
  ServerDB(const char *file);
  /*
   * Destructor for ServerDB
   */
  ~ServerDB();

  /* 
   * Creates node named FILE_NAME after done checking.
   * If IS_DIR is ture, a directory node is created. Stores INSTANCE_NUMBER
   * in the file's meta data. 
   * The creation fails if either the file/dir already exists or the parent
   * directory doesn't exist.
   * 
   * Return Value: true if the creation succeeds, false otherwire.
   */
  bool checkAndCreate(const std::string &file_name, bool is_dir, uint64_t instance_number);

  /* 
   * Opens node named FILE_NAME after done checking.
   * Stores the instance_number of the node's meta data in INSTANCE_NUMBER.
   * The open fails if either the node doesn't exist.
   * 
   * Return Value: true if the open succeeds, false otherwire.
   */
  bool checkAndOpen(const std::string &file_name, uint64_t *instance_number);


  /* 
   * Delete node named FILE_NAME after done checking.
   * Delete fails if file/dir does not exists, dir has children, lock is held
   * or INSTANCE_NUMBER does not match.
   *
   * Return Value: true if the delete succeeds, false otherwire.
   */
  bool checkAndDelete(const std::string &file_name, uint64_t instance_number);

  /*
   * Read content and meta of node FILE_NAME, and stores the results
   * in CONTENT and META.
   * Operation fails if the node does not exists or INSTANCE_NUMBER does 
   * not match.
   *
   * Return Value: true if the read succeeds, false otherwire.
   */
  bool checkAndRead(const std::string &file_name, uint64_t instance_number,
		    std::string *content, MetaData *meta);

  /*
   * Update node FILE_NAME with content 
   * Operation fails if the node does not exists or INSTANCE_NUMBER does 
   * not match.
   *
   * Return Value: true if the update succeeds, false otherwire.
   */
  bool checkAndUpdate(const std::string &file_name, uint64_t instance_number,
		      const std::string &content);

private:
  // Helper functions
  void create(const char *file);
  void open(const char *file);
  void close();
  void sqlexec(const char *fmt, ...);
  std::string getParentName(const std::string &name);
  bool hasName(const std::string &name);

  sqlite3 *db;
};

#endif /* __SERVERDB_H__ */

