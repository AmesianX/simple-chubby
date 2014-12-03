#ifndef __PAXOS_EXECUTE_REPLICATE_HH_INCLUDED__
#define __PAXOS_EXECUTE_REPLICATE_HH_INCLUDED__ 1
#include <cstdlib>

#include "paxos/paxos.hh"

class ReplicaClientSet;
class ReplicaState;

class ExecuteReplicateEngine {
 public:
  ExecuteReplicateEngine(ReplicaState* replica_state,
                         ReplicaClientSet* replica_client_set) {
    replica_state_ = replica_state;
    replica_client_set_ = replica_client_set;
  }
  void replicateCommand(const init_view_request& request);
  std::string replicateCommand(const std::string& request);
 private:
  ReplicaState* replica_state_;
  ReplicaClientSet* replica_client_set_;
};

#endif  // __PAXOS_EXECUTE_REPLICATE_HH_INCLUDED__
