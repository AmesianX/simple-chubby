// Scaffolding originally generated from include/server.x.
// Edit to add functionality.

#include "server/chubby_server.h"

#include "server/serverimpl.hh"

std::unique_ptr<int>
test_version_server::increment(std::unique_ptr<int> arg, xdr::rpc_msg rhdr,
                               xdr::msg_sock *ms)
{
  std::unique_ptr<int> res(new int);
  *res = 0;
  ms->putmsg(xdr_to_msg(rhdr, *arg + 1));
  std::cout << "inc" << std::endl;
  return res;
}

std::unique_ptr<int>
test_version_server::decrement(std::unique_ptr<int> arg, xdr::rpc_msg rhdr,
                               xdr::msg_sock *ms)
{
  std::unique_ptr<int> res(new int);
  *res = 0;
  ms->putmsg(xdr_to_msg(rhdr, *arg - 1));
  std::cout << "dec" << std::endl;
  return res;
}
