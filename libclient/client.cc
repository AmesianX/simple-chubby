
#include <unistd.h>
#include <sys/socket.h>

#include <iostream>
#include <set>
#include <string>

#include <xdrpp/srpc.h>
#include <xdrpp/rpcbind.hh>
#include <xdrpp/socket.h>

#include <include/rpcconfig.h>
#include <include/server.hh>
#include <include/client.h>

using namespace std;
using namespace xdr;

Client::Client()
{
    client = NULL;
}

Client::~Client()
{
    this->close();
}

void
Client::open(const std::string &host)
{
    if (isConnected()) {
        cout << "Already connected!" << endl;
        exit(1);
    }

    auto fd = tcp_connect(host.c_str(), UNIQUE_RPC_PORT);
    client = new srpc_client<api_v1>{fd.release()};
}

void
Client::close()
{
    if (isConnected()) {
        delete client;
        client = NULL;
    }
}

bool
Client::isConnected()
{
    return client != NULL;
}

FileHandler 
Client::fileOpen(const std::string &file_name, Mode mode)
{
    ArgOpen args;
    
    auto r = client->fileOpen(args);

    return r->val();
}

void 
Client::fileClose(const FileHandler &fd)
{
    FileHandler args;
    
    auto r = client->fileClose(args);

    return;
}

bool 
Client::fileDelete(const FileHandler &fd)
{
    FileHandler args;
    
    auto r = client->fileClose(args);

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

/*
bool
Client::create(const std::string &path, const std::string &val)
{
    kvpair args;

    args.key = path;
    args.val = val;

    auto r = client->create(args);
    if (r->discriminant() == 1) {
	// throw a proper exception
	throw ClientException(static_cast<ClientError>(r->errCode()));
    }
    // get return value
    return r->val();
}

bool
Client::remove(const std::string &path)
{
    auto r = client->remove(path);
    if (r->discriminant() == 1) {
	// throw a proper exception
	throw ClientException(static_cast<ClientError>(r->errCode()));
    }
    // get return value
    return r->val();
    return false;
}

std::string
Client::get(const std::string &path)
{
    auto r = client->get(path);
    if (r->discriminant() == 1) {
	// throw a proper exception
	throw ClientException(static_cast<ClientError>(r->errCode()));
    }
    // get return value
    return r->val();
}

void
Client::set(const std::string &path, const std::string &val)
{
    kvpair args;

    args.key = path;
    args.val = val;

    auto r = client->set(args);
    if (r->discriminant() == 1) {
	// throw a proper exception
	throw ClientException(static_cast<ClientError>(r->errCode()));
    }
    return;
}

std::set<string>
Client::list(const string &path)
{
    std::set<string> s;
    auto r = client->list(path); 
    if (r->discriminant() == 1) {
	// throw a proper exception
	throw ClientException(static_cast<ClientError>(r->errCode()));
    }
    auto v = r->val(); // v is a vector of strings  
    for (auto e : v)
	s.insert(e);
    return s;
}

*/
