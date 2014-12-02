// -*- C++ -*-
// Scaffolding originally generated from paxos/paxos.x.
// Edit to add functionality.

#ifndef __XDR_PAXOS_PAXOS_IMPL_HH_INCLUDED__
#define __XDR_PAXOS_PAXOS_IMPL_HH_INCLUDED__ 1

#include "paxos/paxos.hh"

class ReplicaClientSet;
class ReplicaState;
class ExecuteReplicateEngine;

class paxos_v1_server {
 public:
  paxos_v1_server(ReplicaState* replica_state,
                  ReplicaClientSet* replica_client_set,
                  ExecuteReplicateEngine* execute_replicate_engine) {
    replica_state_ = replica_state;
    replica_client_set_ = replica_client_set;
    execute_replicate_engine_ = execute_replicate_engine;
  }
  using rpc_interface_type = paxos_v1;

  std::unique_ptr<replicate_res> replicate(std::unique_ptr<replicate_arg> arg);
  std::unique_ptr<view_change_res> view_change(std::unique_ptr<view_change_arg> arg);
  std::unique_ptr<new_view_res> new_view(std::unique_ptr<new_view_arg> arg);
  std::unique_ptr<init_view_res> init_view(std::unique_ptr<init_view_arg> arg);
 private:
  ReplicaState* replica_state_;
  ReplicaClientSet* replica_client_set_;
  ExecuteReplicateEngine* execute_replicate_engine_;
};

class paxos_client_v1_server {
public:
  using rpc_interface_type = paxos_client_v1;

  std::unique_ptr<execute_res> execute(std::unique_ptr<execute_arg> arg);
};

#endif // !__XDR_PAXOS_PAXOS_IMPL_HH_INCLUDED__
