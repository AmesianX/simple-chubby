#ifndef __PAXOS_CHANGE_VIEW_HH_INCLUDED__
#define __PAXOS_CHANGE_VIEW_HH_INCLUDED__ 1

#include <cstdlib>

#include "paxos/paxos.hh"

class ReplicaClientSet;
class ReplicaState;
class ExecuteReplicateEngine;

class ChangeViewEngine {
  constexpr static double kTimeslice = 1.0;
  constexpr static double kMinTimeslice = 0.5;
 public:
  ChangeViewEngine(ReplicaState* replica_state,
                   ReplicaClientSet* replica_client_set,
                   ExecuteReplicateEngine* execute_replicate_engine);
  void sleep_random();
  bool isLeaderDown();
  // Choose a new leader with the minimum rank.
  int getNewLeaderRank();
  // Returns true if notified another replica, returns false if become leader
  // itself.
  bool NotifyNewLeader();
  void run();
 private:
  ReplicaState* replica_state_;
  ReplicaClientSet* replica_client_set_;
  ExecuteReplicateEngine* execute_replicate_engine_;
};

#endif  // __PAXOS_CHANGE_VIEW_HH_INCLUDED__
