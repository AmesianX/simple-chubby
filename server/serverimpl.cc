// Scaffolding originally generated from include/server.x.
// Edit to add functionality.

#include "server/chubby_server.h"

#include "server/serverimpl.hh"

std::unique_ptr<int>
test_version_server::increment(std::unique_ptr<int> arg,
                               xdr::SessionId session_id, uint32_t xid)
{
  std::unique_ptr<int> res(new int);
  *res = *arg + 1;
  chubby_server_->reply(session_id, xid, std::move(res));
  return res;
}

std::unique_ptr<int>
test_version_server::decrement(std::unique_ptr<int> arg,
                               xdr::SessionId session_id, uint32_t xid)
{
  std::unique_ptr<int> res(new int);
  *res = *arg - 1;
  chubby_server_->reply(session_id, xid, std::move(res));
  return res;
}
