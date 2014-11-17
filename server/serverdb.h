
#ifndef __SERVERDB_H__
#define __SERVERDB_H__

#include "sqlite3.h"

#include <set>
#include <string>
#include <cstdint>

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
     * Checks if a key exists.
     *
     * Return Value: true if the key exists, false otherwise
     */
    bool hasKey(const std::string &path);
    bool hasName(const std::string &name);
    /*
     * Get a value
     *
     * Return Value: value of the key or throws an exception
     */
    std::string get(const std::string &path);
    /*
     * Set or Update a key-value pair
     *
     * Return Value: true if the key already exists, false otherwise
     */
    bool set(const std::string &path, const std::string &val);
    /*
     * Removes a single key
     *
     * Throws an exception if it does not exist
     */
    void remove(const std::string &path);
    /*
     * List all sub-keys
     *
     * Return Value: A set of all sub-keys, returns empty set if it does not 
     * exist or has no children.
     */
    std::set<std::string> list(const std::string &path);

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
     * or INSTANCE_NUMBER is not larger than or equal to node's.
     *
     * Return Value: true if the delete succeeds, false otherwire.
     */
    bool checkAndDelete(const std::string &file_name, uint64_t instance_number);

private:
    // Helper functions
    void create(const char *file);
    void open(const char *file);
    void close();
    void sqlexec(const char *fmt, ...);
    std::string getParentName(const std::string &name);

    sqlite3 *db;
};

#endif /* __SERVERDB_H__ */

