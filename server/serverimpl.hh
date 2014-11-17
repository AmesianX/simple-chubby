// -*- C++ -*-
// Scaffolding originally generated from include/server.x.
// Edit to add functionality.

#ifndef __XDR_SERVER_SERVERIMPL_HH_INCLUDED__
#define __XDR_SERVER_SERVERIMPL_HH_INCLUDED__ 1

#include "include/server.hh"
#include "serverdb.h"
#include <cstdint>
#include <random>
#include <map>
#include <vector>
#include <list>
#include <string>

struct ClientFdPair {
    uint64_t client;
    FileHandler *fd;

    bool operator==(const ClientFdPair &rhs) const {
	return client == rhs.client && fd == rhs.fd;
    }
};

class api_v1_server {
public:
    using rpc_interface_type = api_v1;
    api_v1_server() : db("chubbystore.db"), rand_gen(1234), file2fd_map(), client2fd_map(){ 
	//TODO come from Paxos
	instance_number = 0; 
	master_sequence_number=0;
    }
    
    ~api_v1_server() {}

    std::unique_ptr<RetFd> fileOpen(std::unique_ptr<ArgOpen> arg);
    std::unique_ptr<RetBool> fileClose(std::unique_ptr<FileHandler> arg);
    std::unique_ptr<RetBool> fileDelete(std::unique_ptr<FileHandler> arg);
    std::unique_ptr<RetContentsAndStat> getContentsAndStat(std::unique_ptr<FileHandler> arg);
    std::unique_ptr<RetBool> setContents(std::unique_ptr<ArgSetContents> arg);
    std::unique_ptr<RetBool> acquire(std::unique_ptr<FileHandler> arg);
    std::unique_ptr<RetBool> tryAcquire(std::unique_ptr<FileHandler> arg);
    std::unique_ptr<RetBool> release(std::unique_ptr<FileHandler> arg);

private:
    FileHandler *findFd(uint64_t client_id, const FileHandler &fd);
    bool checkName(const std::string &file_name);

    ServerDB db;
    uint64_t instance_number;
    uint64_t master_sequence_number;
    std::mt19937_64 rand_gen;
    std::map<std::string, std::list<ClientFdPair> > file2fd_map;
    std::map<uint64_t, std::list<FileHandler *> > client2fd_map;
    
};

#endif // !__XDR_SERVER_SERVERIMPL_HH_INCLUDED__
