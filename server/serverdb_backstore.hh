#ifndef __SERVER_SERVERDB_BACKSTOREDB_H__
#define __SERVER_SERVERDB_BACKSTOREDB_H__
#include <cassert>
#include <iostream>
#include "paxos/back_store.hh"
#include "server/serverdb.h"

// This is Paxos's view of ServerDB.
class ServerdbBackstore : public BackStoreInterface {
 public:
  ServerdbBackstore(const char* file) : server_db(file) {}
  virtual void Initialize() override {
    assert(false);
  }
  virtual std::string Run(const std::string input) override;
 private:
  ServerDB server_db;
};  // class ServerdbBackstore.

#endif  // __SERVER_SERVERDB_BACKSTOREDB_H__
