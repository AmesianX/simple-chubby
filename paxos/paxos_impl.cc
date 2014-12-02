// Scaffolding originally generated from paxos/paxos.x.
// Edit to add functionality.

#include "paxos/paxos_impl.hh"

std::unique_ptr<replicate_res>
paxos_v1_server::replicate(std::unique_ptr<replicate_arg> arg)
{
  std::unique_ptr<replicate_res> res(new replicate_res);
  
  // Fill in function body here
  
  return res;
}

std::unique_ptr<view_change_res>
paxos_v1_server::view_change(std::unique_ptr<view_change_arg> arg)
{
  std::unique_ptr<view_change_res> res(new view_change_res);
  
  // Fill in function body here
  
  return res;
}

std::unique_ptr<new_view_res>
paxos_v1_server::new_view(std::unique_ptr<new_view_arg> arg)
{
  std::unique_ptr<new_view_res> res(new new_view_res);
  
  // Fill in function body here
  
  return res;
}

std::unique_ptr<init_view_res>
paxos_v1_server::init_view(std::unique_ptr<init_view_arg> arg)
{
  std::unique_ptr<init_view_res> res(new init_view_res);
  
  // Fill in function body here
  
  return res;
}

std::unique_ptr<execute_res>
paxo_client_v1_server::execute(std::unique_ptr<execute_arg> arg)
{
  std::unique_ptr<execute_res> res(new execute_res);
  
  // Fill in function body here
  
  return res;
}
