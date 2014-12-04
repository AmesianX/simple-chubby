#ifndef __PAXOS_REPLICA_STATE_HH_INCLUDED__
#define __PAXOS_REPLICA_STATE_HH_INCLUDED__ 1

#include <pthread.h>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "paxos/paxos.hh"
/*
struct vc_state {
  enum _mode_t : std::uint32_t {
    VC_ACTIVE,
    VC_MANAGER,
    VC_UNDERLING,
  };

  _mode_t mode{};  // Maintained.
  view_t view{};
};
struct view_t {
  viewid_t vid{};
  cohort_t primary{};  // Maintained.
  xdr::xvector<cohort_t> backups{};
};
struct viewid_t {
  std::uint64_t counter{};
  cid_t manager{};
};
using cid_t = xdr::xstring<>;
struct cohort_t {
  cid_t id{};
  net_address_t addr{};
};
using net_address_t = xdr::xstring<>;
*/
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
