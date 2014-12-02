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
  // No leader is elected yet.
  if (replica_state_->view.primary.addr.empty()) {
    return true;
  }
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
  int result = replica_state_->getSelfRank();
  for (int i = 0; i < replica_state_->getQuota(); ++i) {
    if (i != replica_state_->getSelfRank()) {
      auto* replica_client = replica_client_set_->getReplicaClient(i);
      if (replica_client) {
        if (i < result) {
          result = i;
        }
        replica_client_set_->releaseReplicaClient(i);
        break;
      }
    }
  }
  return result;
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
    std::cout << "Tries to notify the new leader: " << rank << std::endl;
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
      std::cout << "Try to initiate view change." << std::endl;
      // Begin initiate a view change.
      NotifyNewLeader();
      replica_state_->EndAccess();
      continue;
    }
    if (isLeaderDown()) {
      std::cout << "Find Leader is down." << std::endl;
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
