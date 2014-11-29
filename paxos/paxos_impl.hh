// -*- C++ -*-
// Scaffolding originally generated from paxos/paxos.x.
// Edit to add functionality.

#ifndef __XDR_PAXOS_PAXOS_IMPL_HH_INCLUDED__
#define __XDR_PAXOS_PAXOS_IMPL_HH_INCLUDED__ 1

#include "paxos/paxos.hh"

class paxos_v1_server {
public:
  using rpc_interface_type = paxos_v1;

  std::unique_ptr<execute_res> execute(std::unique_ptr<execute_arg> arg);
  std::unique_ptr<replicate_res> replicate(std::unique_ptr<replicate_arg> arg);
  std::unique_ptr<view_change_res> view_change(std::unique_ptr<view_change_arg> arg);
  std::unique_ptr<new_view_res> new_view(std::unique_ptr<new_view_arg> arg);
  std::unique_ptr<init_view_res> init_view(std::unique_ptr<init_view_arg> arg);
};

#endif // !__XDR_PAXOS_PAXOS_IMPL_HH_INCLUDED__