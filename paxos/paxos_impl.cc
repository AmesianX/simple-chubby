// Scaffolding originally generated from paxos/paxos.x.
// Edit to add functionality.

#include "paxos/paxos_impl.hh"
#include "paxos/replica_state.hh"

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
  replica_state_->BeginAccess();
  if (replica_state_->isLeader) {
    replica_state_->EndAccess();
    return res;
  }
  std::cout << "[LEADER] Becomes leader!" << std::endl;
  replica_state_->isLeader = true;
  int rank = replica_state_->getSelfRank();
  replica_state_->view.primary.id =
      replica_state_->getReplicaAddress(rank);
  replica_state_->view.primary.addr =
      replica_state_->getClientUseAddress(rank);
  replica_state_->mode = ReplicaState::VC_ACTIVE;
  // Broadcast itself.
  replica_state_->EndAccess();
  return res;
}

std::unique_ptr<execute_res>
paxos_client_v1_server::execute(std::unique_ptr<execute_arg> arg)
{
  std::unique_ptr<execute_res> res(new execute_res);
  
  // Fill in function body here
  
  return res;
}
