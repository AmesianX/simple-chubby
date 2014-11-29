#include <unistd.h>
#include <sys/socket.h>

#include <exception>
#include <iostream>
#include <string>
#include <thread>

#include <xdrpp/server.h>
#include <xdrpp/srpc.h>
#include <xdrpp/socket.h>

#include "paxos/paxos.hh"
#include "paxos/paxos_impl.hh"

void paxos_listener_thread_entry(const std::string& port) {
  xdr::rpc_tcp_listener paxos_listener(
      xdr::tcp_listen(port.c_str(), AF_INET));
  paxos_v1_server paxos_server;
  try {
    paxos_listener.register_service(paxos_server);
    paxos_listener.run();
  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
  }
}

// auto fd = xdr::tcp_connect(host_port.c_str(), listening_port.c_str());
// auto* client = new xdr::srpc_client<paxos_v1>{fd.release()};

int main(int argc, char* argv[]) {
  std::string listening_port("19899");
  paxos_listener_thread_entry(listening_port);
  std::cerr << "ERROR: listener thread exits!" << std::endl;
  return 0;
}
