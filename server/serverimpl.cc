// Scaffolding originally generated from include/server.x.
// Edit to add functionality.

#include "server/serverimpl.hh"

std::unique_ptr<int>
test_version_server::increment(std::unique_ptr<int> arg)
{
  std::unique_ptr<int> res(new int);
  *res = *arg + 1;
  return res;
}

std::unique_ptr<int>
test_version_server::decrement(std::unique_ptr<int> arg)
{
  std::unique_ptr<int> res(new int);
  *res = *arg - 1;
  return res;
}
