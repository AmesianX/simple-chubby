
#include <unistd.h>
#include <sys/socket.h>

#include <iostream>
#include <thread>

#include <xdrpp/srpc.h>
#include <xdrpp/rpcbind.hh>
#include <xdrpp/socket.h>

#include "include/rpcconfig.h"

#include "serverimpl.hh"

using namespace std;
using namespace xdr;

int main(int argc, const char *argv[])
{
    api_v1_server s;
    rpc_tcp_listener rl(tcp_listen(UNIQUE_RPC_PORT, AF_INET));

    try {
        rl.register_service(s);
        rl.run();
    } catch (exception &e) {
        cerr << e.what() << endl;
    }

    return 0;
}

