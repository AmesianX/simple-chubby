#ifndef __PAXOS_PAXOS_LIB_HH_INCLUDED__
#define __PAXOS_PAXOS_LIB_HH_INCLUDED__ 1
#endif  // __PAXOS_PAXOS_LIB_HH_INCLUDED__ 1

#include "paxos.hh"
#include "paxos/execute_replicate_engine.hh"

class BackStoreInterface {
};

namespace xdr {
class rpc_tcp_listener;
}
class ReplicaState;
class ReplicaClientSet;
class paxos_client_v1_server;
class paxos_v1_server;
class ExecuteReplicateEngine;
class ChangeViewEngine;

class PaxosLib {
 public:
  PaxosLib(const std::string& config_file_name, int rank_id);
  void RegisterBackStore(BackStoreInterface* back_store_input) {
    back_store = back_store_input;
  }
  void Run();
  ~PaxosLib();
  paxos_client_v1_server* paxos_interface_for_user{};
  ExecuteReplicateEngine* execute_replicate_engine{};
 private:
  void RunServer();
  BackStoreInterface* back_store{};
  ReplicaState* replica_state{};
  ReplicaClientSet* replica_client_set{};
  xdr::rpc_tcp_listener* paxos_listener{};
  paxos_v1_server* paxos_server{};
  ChangeViewEngine* change_view_engine{};
};
