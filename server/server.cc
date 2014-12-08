
#include <unistd.h>
#include <sys/socket.h>

#include <iostream>
#include <fstream>
#include <string>
#include <thread>

#include <xdrpp/srpc.h>
#include <xdrpp/socket.h>

#include "include/rpcconfig.h"
#include "server/chubby_server.h"
#include "server/serverdb_backstore.hh"
#include "paxos/paxos_lib.hh"

#include "serverimpl.hh"

using namespace std;
using namespace xdr;

std::string GetServicePort(const std::string& path_name, int self_rank) {
  std::ifstream config_file;
  config_file.open(path_name);
  std::string line;
  std::getline(config_file, line);
  std::getline(config_file, line);
  int max_num = std::stoi(line);
  for (int i = 0; i < max_num; ++i) {
    std::getline(config_file, line);
    std::getline(config_file, line);
    if (i == self_rank) {
      config_file.close();
      return line.substr(line.find(":")+1);
    }
  }
  std::cerr << "rank number invalid." << std::endl;
  assert(false);
  config_file.close();
}

int main(int argc, const char *argv[])
{
    if (argc != 3) {
      std::cout << "./paxos_replica [config_file_name]"
          << "[replica_rank_start_from_0]" << std::endl
          << "config file format:" << std::endl
          << "quota; number_of_clients; "
          << "replica_address; client_address ..." << std::endl;
      return 1;
    }

    int self_rank = std::stoi(argv[2]);
    ServerdbBackstore back_store(
        (std::to_string(self_rank) + "chubbystore.db").c_str());
    PaxosLib paxos_lib(argv[1], self_rank, &back_store);
    std::cout << "Chubby service started on port# " <<
        GetServicePort(argv[1], self_rank) << std::endl;
    xdr::chubby_server chubby_server(
        tcp_listen(GetServicePort(argv[1], self_rank).c_str(), AF_INET),
        &paxos_lib);
    api_v1_server s(&chubby_server, &paxos_lib);
    back_store.RegisterInitializationCallback(
        std::bind(&api_v1_server::initializeLeader, &s));

    std::thread run_thread(std::bind(&PaxosLib::Run, &paxos_lib));
    run_thread.detach();


    try {
        chubby_server.register_service(s);
        chubby_server.run();
    } catch (exception &e) {
        cerr << e.what() << endl;
    }

    return 0;
}

