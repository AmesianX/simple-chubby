#ifndef __SERVER_SERVERDB_BACKSTOREDB_H__
#define __SERVER_SERVERDB_BACKSTOREDB_H__
#include <cassert>
#include <functional>
#include <iostream>
#include "paxos/back_store.hh"
#include "server/serverdb.h"

// This is Paxos's view of ServerDB.
class ServerdbBackstore : public BackStoreInterface {
 public:
  ServerdbBackstore(const char* file) : server_db(file) {}
  virtual void Initialize() override {
    assert(callback_);
    callback_();
  }
  virtual std::string Run(const std::string input) override;
  void RegisterInitializationCallback(const std::function<void()>& callback) {
    callback_ = callback;
  }
 private:
  ServerDB server_db;
  std::function<void()> callback_;
};  // class ServerdbBackstore.

#endif  // __SERVER_SERVERDB_BACKSTOREDB_H__
