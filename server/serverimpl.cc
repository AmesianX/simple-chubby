// Scaffolding originally generated from include/server.x.
// Edit to add functionality.

#include <string>
#include <iostream>

#include "server/serverimpl.hh"

std::unique_ptr<bool>
api_v1_server::create(std::unique_ptr<kvpair> arg)
{
  bool hasKey;
  std::string key = arg->key;
  std::string val = arg->val;
  std::unique_ptr<bool> res(new bool);

  // Fill in additional sanity checking (e.g. prevent malformed paths)

  hasKey = db.hasKey(arg->key);
  if (hasKey) {
    (*res) = false;
    std::cout << "Created " << key << " Failed" << std::endl;
  } else {
    (*res) = true;
    db.set(key, val);
    std::cout << "Created " << key << " Succeded" << std::endl;
  }

  return res;
}

std::unique_ptr<bool>
api_v1_server::remove(std::unique_ptr<longstring> arg)
{
  std::unique_ptr<bool> res(new bool);
  
  // Fill in function body here
  
  return res;
}

std::unique_ptr<bool>
api_v1_server::set(std::unique_ptr<kvpair> arg)
{
  std::unique_ptr<bool> res(new bool);
  
  // Fill in function body here
  
  return res;
}
