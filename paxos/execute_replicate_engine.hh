#ifndef __PAXOS_EXECUTE_REPLICATE_HH_INCLUDED__
#define __PAXOS_EXECUTE_REPLICATE_HH_INCLUDED__ 1
#include <cstdlib>

#include "paxos/back_store.hh"
#include "paxos/paxos.hh"

class ReplicaClientSet;
class ReplicaState;

class ExecuteReplicateEngine {
 public:
  ExecuteReplicateEngine(ReplicaState* replica_state,
                         ReplicaClientSet* replica_client_set,
                         BackStoreInterface* back_store) {
    replica_state_ = replica_state;
    replica_client_set_ = replica_client_set;
    back_store_ = back_store;
  }
  // Replcates initial view change request.
  void replicateCommand(const init_view_request& request);
  // Replcates user command and returns results.
  std::string replicateCommand(const std::string& request);
  // Replcates other replicas up-to-date.
  void replicateCommand() {}
  void BackUp(const std::string& arg) {
    assert(back_store_);
    back_store_->Run(arg);
  }
 private:
  ReplicaState* replica_state_;
  ReplicaClientSet* replica_client_set_;
  BackStoreInterface* back_store_;
};

#endif  // __PAXOS_EXECUTE_REPLICATE_HH_INCLUDED__
