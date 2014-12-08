#include <unistd.h>
#include <sys/socket.h>

#include <exception>
#include <iostream>
#include <string>
#include <thread>

#include <xdrpp/server.h>
#include <xdrpp/srpc.h>
#include <xdrpp/socket.h>

#include "paxos/helper.hh"
#include "paxos/change_view_engine.hh"
#include "paxos/execute_replicate_engine.hh"
#include "paxos/paxos.hh"
#include "paxos/paxos_impl.hh"
#include "paxos/replica_state.hh"
#include "paxos/replica_client_set.hh"

#include "paxos/paxos_lib.hh"

namespace {
std::map<int, net_address_t> ReturnOtherReplicas(
    const ReplicaState& replica_state) {
  std::map<int, net_address_t> other_replicas;
  for (int i = 0; i < replica_state.getMaxNumClient(); ++i) {
    if (i != replica_state.getSelfRank()) {
      other_replicas[i] = replica_state.getReplicaAddress(i);
    }
  }
  return other_replicas;
}
void paxos_listener_thread_entry(
    paxos_v1_server* paxos_server, const std::string& port) {
}
}

PaxosLib::PaxosLib(const std::string& config_file_name, int rank_id,
                   BackStoreInterface* back_store_input) {
  back_store = back_store_input;
  // Config Paxos replica state.
  replica_state = new ReplicaState(config_file_name, rank_id);

  // Config the client side of Paxos inter-replica channels.
  replica_client_set = new ReplicaClientSet(
      ReturnOtherReplicas(*replica_state), replica_state);

  // Config Execute engine.
  execute_replicate_engine = new ExecuteReplicateEngine(
      replica_state, replica_client_set, back_store);

  // Config view-change engine.
  change_view_engine = new ChangeViewEngine(
      replica_state, replica_client_set, execute_replicate_engine);

  // Config Paxos inter-replica interface.
  // Usage: paxos_server->[command](arg).
  paxos_server = new paxos_v1_server(
      replica_state, replica_client_set, execute_replicate_engine);

  // Config Paxos user interface: paxos_interface_for_user->execute(arg).
  paxos_interface_for_user = new paxos_client_v1_server(
      replica_state, execute_replicate_engine);
}

PaxosLib::~PaxosLib() {
  delete back_store;
  delete replica_state;
  delete replica_client_set;
  delete paxos_listener;
  delete paxos_interface_for_user;
  delete paxos_server;
  delete execute_replicate_engine;
  delete change_view_engine;
}

void PaxosLib::RunServer() {
  assert(back_store);
  // Starts the server side of Paxos the inter-replica channels.
  std::string self_replica_address =
      replica_state->getReplicaAddress(replica_state->getSelfRank());
  std::string port = analyzeNetworkPort(self_replica_address);
  paxos_listener = new xdr::rpc_tcp_listener(
      xdr::tcp_listen(port.c_str(), AF_INET));
  printf("Paxos inter-replica channel: listening on port#%s.\n", port.c_str());
  try {
    paxos_listener->register_service(*paxos_server);
    paxos_listener->run();
  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
  }
  std::cerr << "ERROR: listener thread exits!" << std::endl;
  exit(1);
}

void PaxosLib::Run() {
  // Connects to other paxos replicas.
  replica_client_set->tryConnect();

  // Starts the view_change thread.
  std::thread change_view_engine_thread(
      std::bind(&ChangeViewEngine::run, change_view_engine));

  // Starts the server side of Paxos the inter-replica channels.
  std::thread paxos_server_thread(
      std::bind(&PaxosLib::RunServer, this));

  change_view_engine_thread.join();
  paxos_server_thread.join();
}

bool PaxosLib::isLeaderAndInitialized() {
  bool result;
  assert(replica_state);
  replica_state->BeginAccess();
  result = replica_state->isLeader && replica_state->isInitialized;
  replica_state->EndAccess();
  return result;
}
