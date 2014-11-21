
#include <unistd.h>
#include <sys/socket.h>

#include <iostream>
#include <thread>

#include <xdrpp/srpc.h>
#include <xdrpp/socket.h>

#include "include/rpcconfig.h"
#include "server/chubby_server.h"

#include "serverimpl.hh"

using namespace std;
using namespace xdr;

int main(int argc, const char *argv[])
{
    xdr::chubby_server chubby_server(tcp_listen(UNIQUE_RPC_PORT, AF_INET));
    api_v1_server s(&chubby_server);

    try {
        chubby_server.register_service(s);
        chubby_server.run();
    } catch (exception &e) {
        cerr << e.what() << endl;
    }

    return 0;
}

