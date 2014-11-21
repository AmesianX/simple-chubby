// -*- C++ -*-
// Scaffolding originally generated from include/server.x.
// Edit to add functionality.

#ifndef __XDR_SERVER_SERVERIMPL_HH_INCLUDED__
#define __XDR_SERVER_SERVERIMPL_HH_INCLUDED__ 1

#include "serverdb.h"
#include "include/server.hh"

class api_v1_server {
public:
  using rpc_interface_type = api_v1;

  api_v1_server() : db("kvstore.db") { }
  ~api_v1_server() {}

  std::unique_ptr<RPCBool> create(std::unique_ptr<kvpair> arg);
  std::unique_ptr<RPCBool> remove(std::unique_ptr<longstring> arg);
  std::unique_ptr<RPCString> get(std::unique_ptr<longstring> arg);
  std::unique_ptr<RPCBool> set(std::unique_ptr<kvpair> arg);
  std::unique_ptr<RPCSet> list(std::unique_ptr<longstring> arg);
private:
  ServerDB db;
};

#endif // !__XDR_SERVER_SERVERIMPL_HH_INCLUDED__