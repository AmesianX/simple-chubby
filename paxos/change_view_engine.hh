#ifndef __PAXOS_CHANGE_VIEW_HH_INCLUDED__
#define __PAXOS_CHANGE_VIEW_HH_INCLUDED__ 1

#include <cstdlib>

#include "paxos/replica_client_set.hh"
#include "paxos/replica_state.hh"
#include "paxos/paxos.hh"

class ChangeViewEngine {
  constexpr static double kTimeslice = 1.0;
  constexpr static double kMinTimeslice = 0.5;
 public:
  ChangeViewEngine(ReplicaState* replica_state_) {
  }
  void sleep_random() {
    sleep(kMinTimeslice + kTimeslice * rand() / (RAND_MAX));
  }
  bool isLeaderDown() {
    assert(!replica_state_->view.primary.addr.empty());
    int leader_rank =
        replica_state_->getClientUseAddressRank(
            replica_state_->view.primary.addr);
    assert(leader_rank != -1);
    auto* replica_client = replica_client_set_->getReplicaClient(leader_rank);
    if (replica_client) {
      replica_client_set_->releaseReplicaClient(leader_rank);
      return false;
    } else {
      return true;
    }
  }
  void run() {
    while (true) {
      replica_state_->BeginAccess();
      if (replica_state_->mode == ReplicaState::VC_MANAGER) {
        // Begin initiate a view change.
        replica_state_->EndAccess();
        continue;
      }
      if (isLeaderDown()) {
        replica_state_->mode = ReplicaState::VC_MANAGER;
        replica_state_->EndAccess();
        continue;
        // Prepare to initiate a view change after a random time sleep.
        sleep_random();
      }
      replica_state_->EndAccess();
      sleep(kTimeslice);
    }
  }
 private:
  ReplicaState* replica_state_;
  ReplicaClientSet* replica_client_set_;
};

#endif  // __PAXOS_CHANGE_VIEW_HH_INCLUDED__
