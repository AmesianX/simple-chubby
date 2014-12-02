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

void paxos_listener_thread_entry(
    paxos_v1_server* paxos_server, const std::string& port) {
  std::cout << "[CONNECTION] Paxos channel between replica:" << std::endl
      << "    listening on port# " << port << std::endl;

  xdr::rpc_tcp_listener paxos_listener(
      xdr::tcp_listen(
          port.c_str(), AF_INET));
  try {
    paxos_listener.register_service(*paxos_server);
    paxos_listener.run();
  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
  }
  std::cerr << "ERROR: listener thread exits!" << std::endl;
  exit(1);
}

// auto fd = xdr::tcp_connect(host_port.c_str(), listening_port.c_str());
// auto* client = new xdr::srpc_client<paxos_v1>{fd.release()};

int main(int argc, char* argv[]) {
  if (argc != 3) {
    std::cout << "./paxos_replica [config_file_name]"
        << "[replica_rank_start_from_0]" << std::endl
        << "config file format:" << std::endl
        << "quota; replica_address; client_address ..." << std::endl;
    return 1;
  }
  // Config replica state.
  ReplicaState replica_state(argv[1], std::stoi(argv[2]));

  std::map<int, net_address_t> other_replicas;
  for (int i = 0; i < replica_state.getQuota(); ++i) {
    if (i != replica_state.getSelfRank()) {
      other_replicas[i] = replica_state.getReplicaAddress(i);
    }
  }
  // Config the client side of inter-replica channels.
  ReplicaClientSet replica_client_set(other_replicas);

  // User interface: paxos_interface_for_user->execute(arg).
  paxos_client_v1_server paxos_interface_for_user;

  // Connects to other paxos replicas.
  replica_client_set.tryConnect();

  std::string self_replica_address =
      replica_state.getReplicaAddress(replica_state.getSelfRank());

  // Execute engine.
  ExecuteReplicateEngine execute_replicate_engine(
      &replica_state, &replica_client_set);
  // paxos_server.
  paxos_v1_server paxos_server(&replica_state, &replica_client_set,
                               &execute_replicate_engine);

  // Starts the server side of the inter-replica channels.
  std::thread paxos_listener_thread(
      std::bind(
          paxos_listener_thread_entry,
          &paxos_server,
          analyzeNetworkPort(self_replica_address)));
  ChangeViewEngine change_view_engine(&replica_state, &replica_client_set,
                                      &execute_replicate_engine);
  std::thread change_view_engine_thread(
      std::bind(&ChangeViewEngine::run, &change_view_engine));

  //std::thread
  while (true) {
    // Connects to other paxos replicas.
    // replica_client_set.tryConnect();
    // sleep(1);
  }
  return 0;
}
