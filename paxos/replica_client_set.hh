#ifndef __PAXOS_REPLICA_CLIENT_SET_INCLUDED__
#define __PAXOS_REPLICA_CLIENT_SET_INCLUDED__ 1

#include <pthread.h>
#include <unistd.h>
#include <functional>
#include <list>
#include <map>
#include <string>
#include <vector>
#include "xdrpp/socket.h"
#include "xdrpp/srpc.h"
#include "paxos/paxos.hh"

class ReplicaState;

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
      // Copy is invoked, but content is not copied.
      pthread_mutex_init(&lock, nullptr);
      replica_client = nullptr;
    }
    ~ReplicaClientState() {
      pthread_mutex_destroy(&lock);
      delete replica_client;
    }
  };

  // A thread to detect whether the socket peer hangs up the socket.
  void detectFailure(ReplicaClientState* replica_client);

 public:
  ReplicaClientSet(const std::map<int, net_address_t>& other_replicas,
                   ReplicaState* replica_state);
  ~ReplicaClientSet();

  // Tries to connect all the replica client channels.
  void tryConnect();

  // Returns the replica client associated with a rank_id, returns nullptr
  // if the channel is not built yet.
  ReplicaClientType* getReplicaClient(
      int rank_id);
  // Releases the replica client. Because the replica client is single-threaded.
  void releaseReplicaClient(int rank_id);

 private:
  pthread_mutex_t start_connecting_lock_;
  std::map<int, ReplicaClientState> replica_client_list_;
  ReplicaState* replica_state_;
};

#endif  // __PAXOS_REPLICA_CLIENT_SET_INCLUDED__
