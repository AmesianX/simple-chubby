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
#include "paxos/paxos.hh"
#include "paxos/paxos_impl.hh"
#include "paxos/replica_state.hh"
#include "paxos/replica_client_set.hh"

void paxos_listener_thread_entry(const std::string& port) {
  std::cout << "HANG: try to connect " << port << std::endl;
  xdr::rpc_tcp_listener paxos_listener(
      xdr::tcp_listen(port.c_str(), AF_INET));
  std::cout << "HANG: after listen " << port << std::endl;
  paxos_v1_server paxos_server;
  try {
    paxos_listener.register_service(paxos_server);
    paxos_listener.run();
  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
  }
  std::cerr << "ERROR: listener thread exits!" << std::endl;
}

// auto fd = xdr::tcp_connect(host_port.c_str(), listening_port.c_str());
// auto* client = new xdr::srpc_client<paxos_v1>{fd.release()};

int main(int argc, char* argv[]) {
  assert(argc == 3);
  ReplicaState replica_state(argv[1], std::stoi(argv[2]));
  std::map<int, net_address_t> other_replicas;
  for (int i = 0; i < replica_state.getQuota(); ++i) {
    if (i != replica_state.getSelfRank()) {
      other_replicas[i] = replica_state.getReplicaAddress(i);
    }
  }
  ReplicaClientSet replica_client_set(other_replicas);
  replica_client_set.tryConnect();

  std::string self_replica_address =
      replica_state.getReplicaAddress(replica_state.getSelfRank());
  std::thread paxos_listener_thread(
      std::bind(
          paxos_listener_thread_entry,
          analyzeNetworkPort(self_replica_address)));
  while (true) {
    replica_client_set.tryConnect();
    sleep(1);
  }
  return 0;
}
