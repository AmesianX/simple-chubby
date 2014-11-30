#include <unistd.h>
#include <sys/socket.h>

#include <iostream>

#include <xdrpp/srpc.h>
#include <server/chubby_client.h>
#include <xdrpp/socket.h>

#include <include/rpcconfig.h>
#include <include/server.hh>
#include <include/client.h>

using namespace std;
using namespace xdr;

Client::Client()
  : fdList()
{
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
  client = new chubby_client_handler<api_v1>{fd.release()};
}

void Client::close() {
  if (isConnected()) {
    // close all opened FDs
    for(auto fd : fdList)
      client->fileClose(fd);
    fdList.clear();
    
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

FileHandlerId
Client::fileOpen(const std::string &file_name, Mode mode)
{
  ArgOpen args;
  args.name = file_name;
  args.mode = mode;

  auto r = client->fileOpen(args);
  if (r->discriminant() == 0) {
    // returned with a valid FD
    fdList.push_back(r->val());
    return fdList.size(); // FileHandlerId is index+1
  }
  return FAIL; // return FAIL
}

void 
Client::fileClose(const FileHandlerId fd)
{
    FileHandler args;

    if( fd < fdList.size()) {
      args = fdList[fd-1]; // index is FileHandlerId-1
      fdList.erase(fdList.begin() + fd - 1); 
      // fileClose always suceeds
      auto r = client->fileClose(args);
    }
    return;
}

bool 
Client::fileDelete(const FileHandlerId fd)
{
    FileHandler args;

    if(fd < fdList.size())
      args = fdList[fd-1]; // index is FileHandlerId-1
    else
      throw ClientException(static_cast<ClientError>(BAD_ARG));
    
    auto r = client->fileClose(args);
    if (r->discriminant() == 1) {
	// throw a proper exception
	throw ClientException(static_cast<ClientError>(r->errCode()));
    }
    return r->val();
}

bool 
Client::getContentsAndStat(const FileHandler &fd, 
			   FileContent *file_content, MetaData *meta_data)
{
    FileHandler args;
    
    auto r = client->getContentsAndStat(args);
    *file_content = r->val().content;
    *meta_data = r->val().stat;

    return true;
}

bool 
Client::setContents(const FileHandler &fd, const FileContent &file_content)
{
    ArgSetContents args;
    args.fd = fd;
    args.content = file_content;
    
    auto r = client->setContents(args);

    return r->val();
}


void 
Client::acquire(const FileHandler &fd)
{
    FileHandler args;
    
    auto r = client->acquire(args);

    return;
}

bool 
Client::tryAcquire(const FileHandler &fd)
{
    FileHandler args;
    
    auto r = client->tryAcquire(args);

    return r->val();
}

void 
Client::release(const FileHandler &fd)
{
    FileHandler args;
    
    auto r = client->release(args);

    return;
}
