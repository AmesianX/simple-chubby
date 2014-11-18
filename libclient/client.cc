#include <unistd.h>
#include <sys/socket.h>

#include <iostream>

#include <xdrpp/srpc.h>
#include <xdrpp/socket.h>

#include <include/rpcconfig.h>
#include <include/server.hh>
#include <include/client.h>

using namespace std;
using namespace xdr;

Client::Client() {
  client = NULL;
}

Client::~Client() {
  this->close();
}

void Client::open(const std::string &host) {
  if (isConnected()) {
    cout << "Already connected!" << endl;
    exit(1);
  }

  auto fd = tcp_connect(host.c_str(), UNIQUE_RPC_PORT);
  client = new srpc_client<test_version>{fd.release()};
}

void Client::close() {
  if (isConnected()) {
    delete client;
    client = NULL;
  }
}

bool Client::isConnected() {
  return client != NULL;
}

int Client::increment(int input) {
  auto r = client->increment(input);
  return *r;
}

int Client::decrement(int input) {
  auto r = client->decrement(input);
  return *r;
}
