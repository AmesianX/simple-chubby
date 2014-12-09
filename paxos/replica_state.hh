#ifndef __PAXOS_REPLICA_STATE_HH_INCLUDED__
#define __PAXOS_REPLICA_STATE_HH_INCLUDED__ 1

#include <cassert>
#include <pthread.h>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "paxos/paxos.hh"

inline bool operator<(const viewid_t& a, const viewid_t& b) {
  return a.counter < b.counter || (a.counter == b.counter && a.manager < b.manager);
}
inline bool operator==(const viewid_t& a, const viewid_t& b) {
  return a.counter == b.counter && a.manager == b.manager;
}

inline bool isNullView(const view_t& v) {
  return v.vid.counter == 0 && v.vid.manager.empty();
}
inline void setNullView(view_t& v) {
  v.vid.counter = 0;
  v.vid.manager = "";
}

struct ReplicaState : public vc_state {
 public:
  ReplicaState(const std::string& path_name, int self_rank);
  ~ReplicaState() {
    pthread_mutex_destroy(&lock_);
  }
  void BeginAccess() {
    pthread_mutex_lock(&lock_);
  }
  void EndAccess() {
    pthread_mutex_unlock(&lock_);
  }
  // "mode" : _mode_t_.
  // "view.primary" : cohort_t.
  // "view.view_id" : viewid_t.
  // The following access is thread-safe.
  bool isLeader;
  bool toBePromoted{false};
  bool isInitialized{false};
  net_address_t getReplicaAddress(int rank_id) const {
    return replica_address_[rank_id];
  }
  net_address_t getClientUseAddress(int rank_id) const {
    return client_use_address_[rank_id];
  }
  int getSelfRank() const {
    return self_rank_;
  }
  int getConsensusQuota() const {
    return quota_;
  }
  int getMaxNumClient() const {
    return max_num_client_;
  }
  int getReplicaAddressRank(const net_address_t& net_address) const {
    auto iter = std::find(replica_address_.begin(),
                          replica_address_.end(),
                          net_address);
    if (iter == replica_address_.end()) {
      return -1;
    } else {
      return std::distance(replica_address_.begin(), iter);
    }
  }
  int getClientUseAddressRank(const net_address_t& net_address) const {
    auto iter = std::find(client_use_address_.begin(),
                          client_use_address_.end(),
                          net_address);
    if (iter == client_use_address_.end()) {
      return -1;
    } else {
      return std::distance(client_use_address_.begin(), iter);
    }
  }

  cohort_t makeCohort(int rank) {
    cohort_t c;
    c.id = getReplicaAddress(rank);
    c.addr = getClientUseAddress(rank);
    return c;
  }

  void markToPromote(const view_t& v) {
    assert(v.primary.id == getReplicaAddress(self_rank_));
    view.vid = v.vid;
    view.primary = v.primary;
    view.backups = v.backups;
    if (proposed_vid < v.vid) proposed_vid = v.vid;
    toBePromoted = true;
  }

  void doPromote() {
    toBePromoted = false;
    isLeader = true;
    mode = ReplicaState::VC_ACTIVE;
  }

 private:
  pthread_mutex_t lock_;
  int quota_;
  int max_num_client_;
  int self_rank_;
  // Address for replica communication.
  std::vector<net_address_t> replica_address_;
  // Address that accpets client requests.
  std::vector<net_address_t> client_use_address_;
};

#endif  // __PAXOS_REPLICA_STATE_HH_INCLUDED__
