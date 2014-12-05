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

Client::Client() : fdList()
{
  client = NULL;
  nextFdId = 1;
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
    for(auto& kv : fdList)
      client->fileClose(kv.second);
    fdList.clear();
    nextFdId = 1;

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

void
Client::printFdList()
{
  cerr<<"fdList:"<<endl;
  for(auto& kv : fdList){
    auto fd = kv.second;
    cerr<<"\t"<< kv.first
        <<" ("<<fd.file_name<<", "<<fd.instance_number
        <<", "<<fd.magic_number<<")"<<endl;
  }
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
    FileHandler fd = r->val();
    uint64_t fdId = nextFdId++;
    fdList[fdId] = fd;
    //printFdList();
    return fdId;
  }
  return FAIL; // return FAIL
}

void
Client::fileClose(FileHandlerId fdId)
{
  FileHandler args;

  auto it = fdList.find(fdId);
  if(it != fdList.end()) {
    args = it->second;
    fdList.erase(it);
    // fileClose always suceeds
    auto r = client->fileClose(args);
  }
  //printFdList();
  return;
}

bool
Client::fileDelete(FileHandlerId fdId)
{
  FileHandler args;

  auto it = fdList.find(fdId);
  if(it != fdList.end())
    args = it->second;
  else
    throw ClientException(static_cast<ClientError>(BAD_ARG));

  auto r = client->fileDelete(args);
  if (r->discriminant() == 1) {
    // throw a proper exception
    throw ClientException(static_cast<ClientError>(r->errCode()));
  }
  //printFdList();
  return r->val();
}

bool
Client::getContentsAndStat(FileHandlerId fdId,
                           FileContent *file_content, MetaData *meta_data)
{
  FileHandler args;

  auto it = fdList.find(fdId);
  if(it != fdList.end()) {
    args = it->second;
    auto r = client->getContentsAndStat(args);
    if(r->discriminant() == 0) {
      // RPC returned normally
      if (file_content != nullptr)
        *file_content = r->val().content;
      if (meta_data != nullptr)
        *meta_data = r->val().stat;
      return true;
    } else
      throw ClientException(static_cast<ClientError>(r->errCode()));
  }
  return false;
}

bool
Client::setContents(FileHandlerId fdId, const FileContent &file_content)
{
  ArgSetContents args;

  auto it = fdList.find(fdId);
  if(it != fdList.end()) {
    args.fd = it->second;
    args.content = file_content;
    auto r = client->setContents(args);
    if(r->discriminant() == 0)
      return r->val();
    else
      throw ClientException(static_cast<ClientError>(r->errCode()));
  }
  return false;
}

void
Client::acquire(FileHandlerId fdId)
{
  FileHandler args;

  auto it = fdList.find(fdId);
  if(it != fdList.end()) {
    args = it->second;
    auto r = client->acquire(args);
    if(r->discriminant() == 0){
      assert (r->val() == true);
      return;
    }
    else
      throw ClientException(static_cast<ClientError>(r->errCode()));
  }
}

bool
Client::tryAcquire(FileHandlerId fdId)
{
  FileHandler args;

  auto it = fdList.find(fdId);
  if(it != fdList.end()) {
    args = it->second;
    auto r = client->tryAcquire(args);
    if(r->discriminant() == 0){
      return r->val();
    }
    else
      throw ClientException(static_cast<ClientError>(r->errCode()));
  }
}

void
Client::release(FileHandlerId fdId)
{
  FileHandler args;

  auto it = fdList.find(fdId);
  if(it != fdList.end()) {
    args = it->second;
    auto r = client->release(args);
    if(r->discriminant() == 0){
      assert (r->val() == true);
      return;
    }
    else
      throw ClientException(static_cast<ClientError>(r->errCode()));
  }
}

void
Client::register_callback(ChubbyEvent e, EventCallback cb) {
  client->register_callback(e, cb);
}

void
Client::delete_callback(ChubbyEvent e) {
  client->delete_callback(e);
}

