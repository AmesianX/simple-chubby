// -*- C++ -*-
// Scaffolding originally generated from include/server.x.
// Edit to add functionality.

#ifndef __XDR_SERVER_SERVERIMPL_HH_INCLUDED__
#define __XDR_SERVER_SERVERIMPL_HH_INCLUDED__ 1

#include "server/serverdb_paxos.hh"
#include "include/server.hh"
#include "include/event.hh"
#include "serverdb.h"
#include <cstdint>
#include <random>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <list>
#include <string>

class PaxosLib;

struct SessionFdPair {
  xdr::SessionId session;
  FileHandler *fd;
  
  bool operator==(const SessionFdPair &rhs) const {
    return session == rhs.session && fd == rhs.fd;
  }
};
  
namespace xdr {
  class chubby_server;
}  // namespace xdr

class api_v1_server {
public:
  using event_interface = handler_v1;  // Interface for event.
  using rpc_interface_type = api_v1;

  api_v1_server(xdr::chubby_server* server, PaxosLib* paxos_lib)
    : chubby_server_(server), db(paxos_lib),
      paxos_lib_(paxos_lib) {}

  ~api_v1_server() {}

  // for implementing chubby APIs
  std::unique_ptr<RetFd> fileOpen(std::unique_ptr<ArgOpen> arg,
				  xdr::SessionId session_id, uint32_t xid);
  std::unique_ptr<RetBool> fileClose(std::unique_ptr<FileHandler> arg,
				     xdr::SessionId session_id, uint32_t xid);
  std::unique_ptr<RetBool> fileDelete(std::unique_ptr<FileHandler> arg,
				      xdr::SessionId session_id, uint32_t xid);
  std::unique_ptr<RetContentsAndStat> 
  getContentsAndStat(std::unique_ptr<FileHandler> arg,
		     xdr::SessionId session_id, uint32_t xid);
  std::unique_ptr<RetBool> setContents(std::unique_ptr<ArgSetContents> arg,
				       xdr::SessionId session_id, uint32_t xid);
  std::unique_ptr<RetBool> acquire(std::unique_ptr<FileHandler> arg,
				   xdr::SessionId session_id, uint32_t xid);
  std::unique_ptr<RetBool> tryAcquire(std::unique_ptr<FileHandler> arg,
				      xdr::SessionId session_id, uint32_t xid);
  std::unique_ptr<RetBool> release(std::unique_ptr<FileHandler> arg,
				   xdr::SessionId session_id, uint32_t xid);
  std::unique_ptr<RetBool> startSession(std::unique_ptr<longstring> arg,
				   xdr::SessionId session_id, uint32_t xid);
  std::unique_ptr<RetBool> fileReopen(std::unique_ptr<ArgReopen> arg,
				   xdr::SessionId session_id, uint32_t xid);
 


  // for client and server failure handling
  void disconnect(xdr::SessionId session_id); 
  void initializeLeader();

  // for testing message
  std::unique_ptr<int> increment(std::unique_ptr<int> arg,
                                 xdr::SessionId session_id, uint32_t xid);
  std::unique_ptr<int> decrement(std::unique_ptr<int> arg,
                                 xdr::SessionId session_id, uint32_t xid);
  
  
private:
  PaxosLib* paxos_lib_;
  FileHandler *findFd(xdr::SessionId client_id, const FileHandler &fd);
  void printFd();
  void sendLockChangeEvent(const std::string &file_name);
  void sendContentChangeEvent(const std::string &file_name);

  ServerDBPaxos db;
  uint64_t master_sequence_number;
  std::hash<std::string> str_hash;
  
  std::unordered_map<xdr::SessionId, std::string> session2client_map;

  std::unordered_map<std::string, std::list<SessionFdPair> > file2fd_map;
  std::unordered_map<xdr::SessionId, std::list<FileHandler *> > session2fd_map;
  
  std::unordered_map<std::string, 
		     std::list<std::pair <xdr::SessionId, uint32_t> > > file2lockQueue_map;
  std::unordered_map<std::string,
		     std::unordered_set<std::string> > client2heldLock_map;
  std::unordered_map<std::string,
		     std::unordered_set<std::string> > client2ephemeral_map;
  
  std::unordered_map<std::string, std::list<xdr::SessionId> > file2lockChange_map;
  std::unordered_map<std::string, std::list<xdr::SessionId> > file2contentChange_map;

  xdr::chubby_server* chubby_server_;
};

#endif // !__XDR_SERVER_SERVERIMPL_HH_INCLUDED__
