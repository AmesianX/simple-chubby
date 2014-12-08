
#include <unistd.h>
#include <sys/socket.h>

#include <iostream>
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

    ServerdbBackstore back_store(std::stoi(argv[2]) + "chubbystore.db");
    PaxosLib paxos_lib(argv[1], std::stoi(argv[2]), &back_store);
    std::thread run_thread(std::bind(&PaxosLib::Run, &paxos_lib));
    run_thread.detach();

    xdr::chubby_server chubby_server(tcp_listen(UNIQUE_RPC_PORT, AF_INET),
                                     &paxos_lib);
    api_v1_server s(&chubby_server, &paxos_lib);

    try {
        chubby_server.register_service(s);
        chubby_server.run();
    } catch (exception &e) {
        cerr << e.what() << endl;
    }

    return 0;
}

