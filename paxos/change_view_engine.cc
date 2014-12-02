#include "paxos/replica_client_set.hh"
#include "paxos/replica_state.hh"

#include "paxos/change_view_engine.hh"

ChangeViewEngine::ChangeViewEngine(ReplicaState* replica_state,
                                   ReplicaClientSet* replica_client_set) {
  replica_state_ = replica_state;
  replica_client_set_ = replica_client_set;
}

void ChangeViewEngine::sleep_random() {
  sleep(kMinTimeslice + kTimeslice * rand() / (RAND_MAX));
}

bool ChangeViewEngine::isLeaderDown() {
  if (replica_state_->isLeader) {
    return false;
  }
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

// Choose a new leader with the minimum rank.
int ChangeViewEngine::getNewLeaderRank() {
  int result = -1;
  for (int i = 0; i < replica_state_->getQuota(); ++i) {
    if (i != replica_state_->getSelfRank()) {
      auto* replica_client = replica_client_set_->getReplicaClient(i);
      if (replica_client) {
        result = i;
        replica_client_set_->releaseReplicaClient(i);
        break;
      }
    }
  }
  if (result == -1) {
    return replica_state_->getSelfRank();
  } else {
    return result;
  }
}

void ChangeViewEngine::NotifyNewLeader() {
  // Cannot handle if the leader wakes up again.
  assert(isLeaderDown());
  int rank = getNewLeaderRank();
  if (rank == replica_state_->getSelfRank()) {
    // Becomes leader itself.
    replica_state_->isLeader = true;
    replica_state_->view.primary.id =
        replica_state_->getReplicaAddress(rank);
    replica_state_->view.primary.addr =
        replica_state_->getClientUseAddress(rank);
    replica_state_->mode = ReplicaState::VC_ACTIVE;
    std::cout << "[LEADER] Becomes leader!" << std::endl;
    // Broadcast itself.
  } else {
    auto* replica_client = replica_client_set_->getReplicaClient(rank);
    assert(replica_client);
    init_view_arg init_view_instruction;
    // Notifies the new leader.
    replica_client->init_view(init_view_instruction);
    replica_client_set_->releaseReplicaClient(rank);
    replica_state_->mode = ReplicaState::VC_ACTIVE;
  }
}

void ChangeViewEngine::run() {
  while (true) {
    replica_state_->BeginAccess();
    if (replica_state_->mode == ReplicaState::VC_MANAGER) {
      // Begin initiate a view change.
      NotifyNewLeader();
      replica_state_->EndAccess();
      continue;
    }
    if (isLeaderDown()) {
      replica_state_->mode = ReplicaState::VC_MANAGER;
      replica_state_->EndAccess();
      // Prepare to initiate a view change after a random time sleep.
      sleep_random();
      continue;
    }
    replica_state_->EndAccess();
    sleep(kTimeslice);
  }
}
