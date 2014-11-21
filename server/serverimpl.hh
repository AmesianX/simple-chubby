// -*- C++ -*-
// Scaffolding originally generated from include/server.x.
// Edit to add functionality.

#ifndef __XDR_SERVER_SERVERIMPL_HH_INCLUDED__
#define __XDR_SERVER_SERVERIMPL_HH_INCLUDED__ 1

#include "include/server.hh"

namespace xdr {
class chubby_server;
};
class test_version_server {
public:
  test_version_server(xdr::chubby_server* server) : chubby_server_(server) {}
  using rpc_interface_type = test_version;

  std::unique_ptr<int> increment(std::unique_ptr<int> arg,
                                 xdr::SessionId session_id, uint32_t xid);
  std::unique_ptr<int> decrement(std::unique_ptr<int> arg,
                                 xdr::SessionId session_id, uint32_t xid);

private:
  xdr::chubby_server* chubby_server_;
};

#endif // !__XDR_SERVER_SERVERIMPL_HH_INCLUDED__
