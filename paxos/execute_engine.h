#ifndef __PAXOS_EXECUTE_ENGINE_INCLUDED__
#define __PAXOS_EXECUTE_ENGINE_INCLUDED__ 1

#include "paxos/paxos.hh"
typedef ReceiveFromUserHandler;
typedef ReceiveFromReplicaHandler;
typedef SendToReplicaHandler;
class ExecuteEngine {
 public:
  ExecuteEngine() {}
  ~ExecuteEngine() {}
  // Runs the command and returns the result. Returns empty pointer if fails.
  xdr::opaque_vect<>* executeCommand(
      std::unique_ptr<xdr::execute_arg> execute_arg);
  // Kills all the sending.
  void killEngine();
};

#endif  // __PAXOS_EXECUTE_ENGINE_INCLUDED__
