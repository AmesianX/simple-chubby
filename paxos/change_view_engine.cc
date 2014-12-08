#include "paxos/replica_client_set.hh"
#include "paxos/replica_state.hh"
#include "paxos/execute_replicate_engine.hh"

#include "paxos/change_view_engine.hh"

ChangeViewEngine::ChangeViewEngine(ReplicaState* replica_state,
                                   ReplicaClientSet* replica_client_set,
                                   ExecuteReplicateEngine*
                                   execute_replicate_engine) {
  replica_state_ = replica_state;
  replica_client_set_ = replica_client_set;
  execute_replicate_engine_ = execute_replicate_engine;
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
  for (int i = 0; i < replica_state_->getMaxNumClient(); ++i) {
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

bool ChangeViewEngine::NotifyNewLeader() {
  // Cannot handle if the leader wakes up again.
  assert(isLeaderDown());
  int rank = getNewLeaderRank();
  if (rank == replica_state_->getSelfRank()) {
    // Becomes leader itself.
    // TODO: only isLeader, view.primary.id, view.primary.addr is maintained.
    replica_state_->isLeader = true;
    replica_state_->view.primary.id =
        replica_state_->getReplicaAddress(rank);
    replica_state_->view.primary.addr =
        replica_state_->getClientUseAddress(rank);
    replica_state_->mode = ReplicaState::VC_ACTIVE;
    printf("[PAXOS] Promoted itself to Paxos leader, rank#%d.\n", rank);
    return true;
  } else {
    printf("[PAXOS] Figured out new Paxos leader, rank#%d.\n", rank);
    auto* replica_client = replica_client_set_->getReplicaClient(rank);
    assert(replica_client);
    init_view_arg init_view_instruction;
    // Notifies the new leader.
    auto result = replica_client->init_view(init_view_instruction);
    if (!result->succeed) {
      replica_state_->isLeader = false;
      replica_state_->view.primary.id = replica_state_->getReplicaAddress(rank);
      replica_state_->view.primary.addr =
          replica_state_->getClientUseAddress(rank);
    }
    replica_client_set_->releaseReplicaClient(rank);
    replica_state_->mode = ReplicaState::VC_ACTIVE;
    return false;
  }
}

void ChangeViewEngine::run() {
  while (true) {
    replica_state_->BeginAccess();
    if (replica_state_->mode == ReplicaState::VC_MANAGER) {
      std::cout << "[PAXOS] Initiate a view change." << std::endl;
      // Begin initiate a view change.
      bool becomeLeader = NotifyNewLeader();
      init_view_request command;
      command.newview = replica_state_->view;
      replica_state_->EndAccess();
      if (becomeLeader) {
        execute_replicate_engine_->replicateCommand(command);
        execute_replicate_engine_->initializeLeader();
      }
      continue;
    }
    if (isLeaderDown()) {
      //std::cout << "Cannot talk with Paxos leader." << std::endl;
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
