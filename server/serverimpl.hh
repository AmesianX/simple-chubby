// -*- C++ -*-
// Scaffolding originally generated from include/server.x.
// Edit to add functionality.

#ifndef __XDR_SERVER_SERVERIMPL_HH_INCLUDED__
#define __XDR_SERVER_SERVERIMPL_HH_INCLUDED__ 1

#include "include/server.hh"

class test_version_server {
public:
  using rpc_interface_type = test_version;

  std::unique_ptr<int> increment(std::unique_ptr<int> arg, xdr::rpc_msg rhdr,
                                 xdr::msg_sock *ms);
  std::unique_ptr<int> decrement(std::unique_ptr<int> arg, xdr::rpc_msg rhdr,
                                 xdr::msg_sock *ms);
};

#endif // !__XDR_SERVER_SERVERIMPL_HH_INCLUDED__
