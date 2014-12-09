#include <unordered_set>
#include "paxos/replica_client_set.hh"
#include "paxos/replica_state.hh"
#include "paxos/execute_replicate_engine.hh"

#include "paxos/change_view_engine.hh"

struct cohort_hash {
  size_t operator()(const cohort_t& c) const {
    return std::hash<std::string>()(std::string(c.id) + std::string(c.addr));
  }
};

static bool operator==(const cohort_t& a, const cohort_t& b) {
  return a.id == b.id && a.addr == b.addr;
}
static bool operator!=(const cohort_t& a, const cohort_t& b) {
  return !(a == b);
}

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

#if 0
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
#endif

void ChangeViewEngine::run() {
  while (true) {
    replica_state_->BeginAccess();
    if (replica_state_->toBePromoted) {
      std::cout << "[PAXOS] Promoted to Paxos leader, rank#" << replica_state_->getSelfRank() << std::endl;

      replica_state_->doPromote();
      init_view_request command;
      command.newview = replica_state_->view;
      replica_state_->EndAccess();

      execute_replicate_engine_->replicateCommand(command);
      execute_replicate_engine_->initializeLeader();
    }
    if (replica_state_->mode == ReplicaState::VC_MANAGER) {
      std::cout << "[PAXOS] Initiate a view change." << std::endl;
      // Begin initiate a view change.

      int self_rank = replica_state_->getSelfRank();
      replica_state_->proposed_vid.counter += 1;
      replica_state_->proposed_vid.manager = replica_state_->getReplicaAddress(self_rank);
      view_change_arg vc_arg;
      vc_arg.oldview = replica_state_->view;
      vc_arg.newvid.counter = replica_state_->proposed_vid.counter;
      vc_arg.newvid.manager = replica_state_->proposed_vid.manager;
      // send view change RPCs
      view_t prev_accepted_view;
      setNullView(prev_accepted_view);
      bool abort = false;
      std::unordered_set<cohort_t, cohort_hash> accepted_set;
      accepted_set.insert(replica_state_->makeCohort(self_rank));
      for (int i = 0; i < replica_state_->getMaxNumClient(); ++i) {
        if (i != self_rank) {
          auto* replica_client = replica_client_set_->getReplicaClient(i);
          if (replica_client) {
            auto r = replica_client->view_change(vc_arg);
            replica_client_set_->releaseReplicaClient(i);
            if (r->accepted()) {
              accepted_set.insert(replica_state_->makeCohort(i));
              // get highest previous accepted view if exists
              if (!isNullView(r->accept().newview)) {
                if (isNullView(prev_accepted_view)
                    || prev_accepted_view.vid < r->accept().newview.vid) {
                  prev_accepted_view = r->accept().newview;
                }
              }
            } else {
              // abort
              abort = true;
              // update state
              if (replica_state_->view.vid < r->reject().oldview.vid) {
                replica_state_->view = r->reject().oldview;
              }
              if (replica_state_->proposed_vid < r->reject().newvid) {
                replica_state_->proposed_vid = r->reject().newvid;
              }
            }
          }
        }
      }

      /* Get \c accepted_set, \c prev_accepted_view, and \c abort at this point
       */

      auto checkQuorum = [](std::unordered_set<cohort_t, cohort_hash> accepted_set, view_t view) {
        auto& primary = view.primary;
        if (primary.addr.empty())
          return true;
        auto& backups = view.backups;
        uint32_t n = 0;
        for (auto& b : backups) {
          if (accepted_set.count(b) != 0)
            n++;
        }
        if (accepted_set.count(primary) != 0)
          n++;

        return n >= (backups.size() + 1) / 2 + 1;
      };

      // check quota in oldview
      if (!abort) {
        abort = !checkQuorum(accepted_set, replica_state_->view);
      }
      // check quota in prev_accepted_view
      if (!abort && !isNullView(prev_accepted_view)) {
        abort = !checkQuorum(accepted_set, prev_accepted_view);
      }

      if (abort) {
        replica_state_->EndAccess();
        continue;
      }

      // TODO: maintain view primary and backups
      std::cout << "[PAXOS] Majority accept the new view id." << std::endl;

      new_view_arg nv_arg{};
      nv_arg.view.vid = vc_arg.newvid;
      if (!isNullView(prev_accepted_view)) {
        // use previous accepted view
        // choose primary, pp 10, right, paragraph 3
        nv_arg.view.primary = prev_accepted_view.primary;
        nv_arg.view.backups = prev_accepted_view.backups;
        if (accepted_set.count(nv_arg.view.primary) == 0) {
          // primary not respond, change a responsive one
          for (int i = 0; i < nv_arg.view.backups.size(); i++) {
            auto& b = nv_arg.view.backups[i];
            if (accepted_set.count(b) != 0) {
              auto temp = b;
              b = nv_arg.view.primary;
              nv_arg.view.primary = temp;
              break;
            }
          }
        }
      } else {
        // choose primary, pp 9, right, paragraph 3
        if (accepted_set.count(replica_state_->view.primary) != 0)
          nv_arg.view.primary = replica_state_->view.primary;
        else
          nv_arg.view.primary = replica_state_->makeCohort(self_rank);
        // set backups
        accepted_set.erase(nv_arg.view.primary);
        nv_arg.view.backups.insert(nv_arg.view.backups.end(), accepted_set.begin(), accepted_set.end());
      }
      accepted_set.clear();  // accepted set is already invalid

      for (int i = 0; i < replica_state_->getMaxNumClient(); ++i) {
        auto dst = replica_state_->makeCohort(i);
        if (dst != nv_arg.view.primary
            && std::find(nv_arg.view.backups.begin(), nv_arg.view.backups.end(), dst) == nv_arg.view.backups.end()) {
          // not involved
          continue;
        }
        if (i != self_rank) {
          auto* replica_client = replica_client_set_->getReplicaClient(i);
          if (replica_client) {
            auto r = replica_client->new_view(nv_arg);
            replica_client_set_->releaseReplicaClient(i);
            if (r->accepted)
              accepted_set.insert(replica_state_->makeCohort(i));
          }
        } else {
          accepted_set.insert(replica_state_->makeCohort(self_rank));
        }
      }

      if (!checkQuorum(accepted_set, nv_arg.view)) {
        replica_state_->EndAccess();
        continue;
      }

      int leader_rank = replica_state_->getClientUseAddressRank(nv_arg.view.primary.addr);
      std::cout << "[PAXOS] New view formed and notify the new leader, rank#" << leader_rank << std::endl;

      if (leader_rank == self_rank) {
        // I am the new leader
        replica_state_->markToPromote(nv_arg.view);
        replica_state_->EndAccess();
        continue;
      } else {
        // notify the new leader
        auto* replica_client = replica_client_set_->getReplicaClient(leader_rank);
        if (replica_client) {
          init_view_arg iv_arg;
          iv_arg.view = nv_arg.view;
          auto r = replica_client->init_view(iv_arg);
          replica_client_set_->releaseReplicaClient(leader_rank);
        }
        replica_state_->EndAccess();

        sleep(kTimeslice);
        continue;
      }
    }
    if (isLeaderDown() && replica_state_->mode != ReplicaState::VC_UNDERLING) {
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
