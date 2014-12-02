#ifndef __PAXOS_REPLICA_CLIENT_SET_INCLUDED__
#define __PAXOS_REPLICA_CLIENT_SET_INCLUDED__ 1

#include <pthread.h>
#include <unistd.h>
#include <functional>
#include <list>
#include <string>
#include <vector>
#include "xdrpp/socket.h"
#include "xdrpp/srpc.h"
#include "paxos/paxos.hh"

class ReplicaClientSet {
 public:
  typedef xdr::srpc_client<paxos_v1> ReplicaClientType;

 private:
  struct ReplicaClientState {
    net_address_t network_address;
    xdr::unique_fd fd;
    ReplicaClientType* replica_client;
    pthread_mutex_t lock;
    ReplicaClientState() {
      pthread_mutex_init(&lock, nullptr);
      replica_client = nullptr;
    }
    ReplicaClientState(const ReplicaClientState&) {
      pthread_mutex_init(&lock, nullptr);
      replica_client = nullptr;
    }
    ~ReplicaClientState() {
      pthread_mutex_destroy(&lock);
      delete replica_client;
    }
  };

  // Breaks the network address to host name and port name.
  std::pair<std::string, std::string> analyzeNetworkAddress(
      const net_address_t& network_address);

  // A thread to detect whether the socket peer hangs up the socket.
  void detectFailure(ReplicaClientState* replica_client);

 public:
  ReplicaClientSet(std::vector<net_address_t> other_replicas);
  ~ReplicaClientSet();

  // Try to connect all the replica client channel.
  void tryConnect();

  // Returns the size of all replica client channel.
  int getSize() const {
    return replica_client_list_.size();
  }

  // Returns the replica client associated with a rank_id, returns nullptr
  // if the channel is not built yet.
  ReplicaClientType* getReplicaClient(
      int rank_id);
  // Release the replica client. Because the replica client is single-threaded.
  void releaseReplicaClient(int rank_id);

 private:
  pthread_mutex_t start_connecting_lock_;
  std::vector<ReplicaClientState> replica_client_list_;
};

#endif  // __PAXOS_REPLICA_CLIENT_SET_INCLUDED__
