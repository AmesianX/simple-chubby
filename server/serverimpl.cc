// Scaffolding originally generated from include/server.x.
// Edit to add functionality.

#include <cstdint>
#include <cassert>

#include "server/chubby_server.h"

#include "server/serverimpl.hh"

const uint64_t READ = 0x1;
const uint64_t WRITE = 0x2;
const uint64_t CREATE_DIRECTORY = 0x4;
const uint64_t CREATE_FILE = 0x8;

const uint64_t EPHEMERAL = 0x10;

const uint64_t EV_LOCK_CHANGED = 0x100;
const uint64_t EV_CONTENT_MODIFIED = 0x200;

enum ClientError {
  BAD_ARG,
  FS_FAIL,
  PAXOS_ERROR
};

using std::cout;
using std::endl;

bool checkName(const std::string &key);

void
api_v1_server::printFd()
{
  cout << "\tfile2fd_map:" << endl;
  for (auto it = file2fd_map.begin(); 
       it != file2fd_map.end(); it++) {
    cout << "\t\t file: "<< it->first<<endl;
    for (auto pair : it->second)
      cout<< "\t\t\t session: "<<pair.session
	  << ", FD: ("<< (pair.fd)->file_name
	  << ", "<< (pair.fd)->instance_number<<")"<<endl;
  }

  cout << "\tsession2fd_map:" <<endl;
  for (auto it = session2fd_map.begin(); 
       it != session2fd_map.end(); it++) {
    cout << "\t\t session: "<< it->first <<endl;
    for (auto fd : it->second)
      cout << "\t\t\t FD: ("<< fd ->file_name
	   << ", "<< fd->instance_number<<")"<<endl;
  }

  cout << "\tfile2lockQueue_map:" <<endl;
  for (auto it = file2lockQueue_map.begin();
       it != file2lockQueue_map.end(); it++) {
    cout << "\t\t file: "<< it->first<<endl;
    for (auto& s : it->second)
      cout << "\t\t\t Session: ("<< s.first
	   << ", "<< s.second <<")"<<endl;
  }

  cout << "\tclient2heldLock_map:" <<endl;
  for (auto it = client2heldLock_map.begin();
       it != client2heldLock_map.end(); it++) {
    cout << "\t\t session: "<< it->first <<endl;
    if (!it->second.empty()) {
      cout << "\t\t\t locks: ";
	for (auto& s : it->second)
	  cout<< s <<", ";
      cout << endl;
    }
  }

  cout << "\tclient2ephemeral_map:" <<endl;
  for (auto it = client2ephemeral_map.begin();
       it != client2ephemeral_map.end(); it++) {
    cout << "\t\t session: "<< it->first <<endl;
    if (!it->second.empty()) {
      cout << "\t\t\t file: ";
	for (auto& s : it->second)
	  cout<< s <<", ";
      cout << endl;
    }
  }

}

std::string
getParentName(const std::string &key)
{
  // find the lenth of the parent of KEY
  int pos = key.length() - 1;
  while (pos > 0) { /* if pos==0, the parent is root, 
		       then break and return "/" */
    if (key[pos] == '/') break;
    --pos;
  }
  if (pos == 0)
    return "/";
  else 
    return std::string(key, 0, pos);  // create a substring of key
}

std::unique_ptr<int>
api_v1_server::increment(std::unique_ptr<int> arg,
                         xdr::SessionId session_id, uint32_t xid)
{
  std::unique_ptr<int> res(new int);
  *res = *arg + 1;
  EventContent evc;
  evc.event = ChubbyEvent::NOP;
  evc.fname = std::string("inc");
  chubby_server_->send<event_interface::event_callback_t>(session_id, evc);
  chubby_server_->reply(session_id, xid, std::move(res));
  evc.fname = std::string("inc done");
  chubby_server_->send<event_interface::event_callback_t>(session_id, evc);
  return res;
}

std::unique_ptr<int>
api_v1_server::decrement(std::unique_ptr<int> arg,
                         xdr::SessionId session_id, uint32_t xid)
{
  std::unique_ptr<int> res(new int);
  *res = *arg - 1;
  EventContent evc;
  evc.event = ChubbyEvent::NOP;
  evc.fname = std::string("dec");
  chubby_server_->send<event_interface::event_callback_t>(session_id, evc);
  chubby_server_->reply(session_id, xid, std::move(res));
  evc.fname = std::string("dec done");
  chubby_server_->send<event_interface::event_callback_t>(session_id, evc);
  return res;
}

std::unique_ptr<RetFd>
api_v1_server::fileOpen(std::unique_ptr<ArgOpen> arg,
                        xdr::SessionId session_id, uint32_t xid)
{
  std::unique_ptr<RetFd> res(new RetFd);
  std::string file_name = arg->name;
  Mode mode = arg->mode;
  std::string client_id = session2client_map[session_id]; 
  assert(!client_id.empty());

  cout<<"\nserver: fileOpen: ("<< file_name << ", "<< mode <<")"<<endl;

  if(!checkName(file_name)) {
    // return with error
    res->discriminant(1);
    res->errCode() = BAD_ARG;
    chubby_server_->reply(session_id, xid, std::move(res));
    return res;
  }   
  
  if((mode & CREATE_DIRECTORY) && (mode & CREATE_FILE)) {
    // return with error
    res->discriminant(1);
    res->errCode() = BAD_ARG;
    chubby_server_->reply(session_id, xid, std::move(res));
    return res;
  }
  
  FileHandler *fd = new FileHandler();
  try {
    // Create file or dir
    if((mode & CREATE_DIRECTORY) || (mode & CREATE_FILE)) {
      bool is_dir = mode & CREATE_DIRECTORY;
      if(!db.checkAndCreate(file_name, is_dir, &(fd->instance_number)) ) {
	// creation failed, then return false
	delete fd;
	res->discriminant(1);
	res->errCode() = FS_FAIL;
	chubby_server_->reply(session_id, xid, std::move(res));
	return res;
      }
      if (mode & EPHEMERAL)
	client2ephemeral_map[client_id].insert(file_name);
    } else { // open an existing file or dir
      // check file is exist
      if(!db.checkAndOpen(file_name, &(fd->instance_number))) {
	// open failed, then return false
	delete fd;
	res->discriminant(1);
	res->errCode() = FS_FAIL;
	chubby_server_->reply(session_id, xid, std::move(res));
	return res;      
      }
    }
  } catch (std::exception &e) {
    delete fd;
    std::cerr<<"\nfileOpen() catch an expection: "<<e.what()<<endl;
    res->discriminant(1);
    res->errCode() = PAXOS_ERROR;
    chubby_server_->reply(session_id, xid, std::move(res));
    return res;
  }

  fd->master_sequence_number = this->master_sequence_number;
  fd->file_name = file_name;
  fd->write_is_allowed = mode & WRITE;
  fd->magic_number = str_hash(fd->file_name +
			      std::to_string(fd->instance_number) +
			      std::to_string(fd->write_is_allowed));  // use hash to sign

  // add FD to <file, list of (session, FD) pairs> 
  file2fd_map[file_name].push_back({session_id, fd});
  // add FD to <session, list of FDs> map
  session2fd_map[session_id].push_back(fd);

  // register events
  if (mode & EV_LOCK_CHANGED)
    file2lockChange_map[file_name].push_back(session_id);
  if (mode & EV_CONTENT_MODIFIED)
    file2contentChange_map[file_name].push_back(session_id);
  
  // return normally with FD
  res->discriminant(0);
  res->val() = *fd;
  printFd();
  chubby_server_->reply(session_id, xid, std::move(res));
  
  // send content change events for parent node
  std::string parent_name = getParentName(fd->file_name);
  if (parent_name.compare("/") != 0 )
    sendContentChangeEvent(parent_name);
  return res;
}

std::unique_ptr<RetBool>
api_v1_server::fileClose(std::unique_ptr<FileHandler> arg,
			 xdr::SessionId session_id, uint32_t xid)
{
  std::unique_ptr<RetBool> res(new RetBool);
  std::string client_id = session2client_map[session_id];
  assert(!client_id.empty());
  
  cout<<"\nserver: fileClose: ("<< arg->file_name << ", "<< arg->instance_number<<")"<<endl;

  FileHandler *fd = findFd(session_id, *arg);
  if(fd == nullptr) {
    // No match FD found
    // return normally with TRUE value
    assert(xid != (uint32_t) -1);
    res->discriminant(1);
    res->errCode() = BAD_ARG;
    chubby_server_->reply(session_id, xid, std::move(res));
    return res;
  }

  // reclaim the locks if held
  if( client2heldLock_map.count(client_id) > 0
      && client2heldLock_map[client_id].count(fd->file_name) > 0 ) {
    std::unique_ptr<FileHandler> tmp_fd(new FileHandler);
    *tmp_fd = *fd;
    release(std::move(tmp_fd), session_id, (uint32_t) -1);
  }

  // delete the node if it is ephemeral
  if( client2ephemeral_map[client_id].count(fd->file_name) > 0) {
    std::unique_ptr<FileHandler> tmp_fd(new FileHandler);
    *tmp_fd = *fd;

    try {
      fileDelete(std::move(tmp_fd), session_id, (uint32_t) -1);
    } catch (std::exception &e) {
      std::cerr<<"\nfileClose() catch an expection: "<<e.what()<<endl;
      res->discriminant(1);
      res->errCode() = PAXOS_ERROR;
      chubby_server_->reply(session_id, xid, std::move(res));
      return res;
    }

    // return normally with TRUE value
    res->discriminant(0);
    res->val() = true;
    printFd();
    chubby_server_->reply(session_id, xid, std::move(res));
    return res;
  }

  // remove FD from <file, list of (session, FD) pairs> map
  file2fd_map[fd->file_name].remove({session_id, fd});
  if(file2fd_map[fd->file_name].empty())
    file2fd_map.erase(fd->file_name);

  // remove FD from <session, list of FDs> map
  session2fd_map[session_id].remove(fd);
  if(session2fd_map[session_id].empty())
    session2fd_map.erase(session_id);


  // clear lock queue
  if(file2lockQueue_map.count(fd->file_name) > 0) {
    auto queue = file2lockQueue_map[fd->file_name];
    // remove the elemet, whose session_id is SESSION_ID
    auto it = queue.begin();
    while(it != queue.end()) {
      if (it->first == session_id)
	break;
    }
    if (it != queue.end())
      queue.erase(it);

    if(queue.empty())
      file2lockQueue_map.erase(fd->file_name);
  }

  // clear event maps
  if(file2lockChange_map.count(fd->file_name) > 0) {
    file2lockChange_map[fd->file_name].remove(session_id);
    if(file2lockChange_map[fd->file_name].empty())
      file2lockChange_map.erase(fd->file_name);
  }
  if(file2contentChange_map.count(fd->file_name) > 0) {
    file2contentChange_map[fd->file_name].remove(session_id);
    if(file2contentChange_map[fd->file_name].empty())
      file2contentChange_map.erase(fd->file_name);
  }

  // delete FD
  delete fd;
  
  // return normally with TRUE value
  res->discriminant(0);
  res->val() = true;
  printFd();
  chubby_server_->reply(session_id, xid, std::move(res));
  
  return res;
}

std::unique_ptr<RetBool>
api_v1_server::fileDelete(std::unique_ptr<FileHandler> arg,
                          xdr::SessionId session_id, uint32_t xid)
{
  std::unique_ptr<RetBool> res(new RetBool);
  std::string client_id = session2client_map[session_id];
  assert(!client_id.empty());

  cout<<"\nserver: fileDelete: ("<< arg->file_name
      << ", "<< arg->instance_number<<")"<<endl;
  
  FileHandler *fd = findFd(session_id, *arg);
  if(fd == nullptr) {
    // No match FD found
    // return an error
    res->discriminant(1);
    res->errCode() = BAD_ARG;
    chubby_server_->reply(session_id, xid, std::move(res));
    return res;
  }
  
  try {
    // try to delete in the database
    if(!db.checkAndDelete(fd->file_name, fd->instance_number)) {
      // Delete failed, return false
      res->discriminant(0);
      res->val() = false;
      chubby_server_->reply(session_id, xid, std::move(res));
      return res;
    }
  } catch (std::exception &e) {
    std::cerr<<"\nfileDelete() catch an expection: "<<e.what()<<endl;
    if (xid != (uint64_t) -1) {
      res->discriminant(1);
      res->errCode() = PAXOS_ERROR;
      chubby_server_->reply(session_id, xid, std::move(res));
      return res;
    } else throw e;
  }

  // remove all FDs in file2fd_map[fd->file_name]
  std::list<SessionFdPair> l = file2fd_map[arg->file_name];
  for (auto it = l.begin(); it != l.end(); ++it) {
    auto s = it->session;
    FileHandler *f = it->fd;
    // remove this FD in session2fd_map
    session2fd_map[s].remove(f);
    if(session2fd_map[s].empty())
      session2fd_map.erase(s);
    // free space
    delete f;
  }  
 
  // remove the list in file2fd_map
  int r = file2fd_map.erase(arg->file_name);
  assert(r == 1);

  // clear lock queue
  file2lockQueue_map.erase(arg->file_name);

  client2ephemeral_map[client_id].erase(fd->file_name);
  if(client2ephemeral_map[client_id].empty())
    client2ephemeral_map.erase(client_id);

  // return normally with TRUE value
  res->discriminant(0);
  res->val() = true;
  printFd();
  chubby_server_->reply(session_id, xid, std::move(res));
  
  // send content change events for current node (event list deleted inside)
  sendContentChangeEvent(arg->file_name);
  // send content change events for parent node
  std::string parent_name = getParentName(arg->file_name);
  if (parent_name.compare("/") != 0 )
    sendContentChangeEvent(parent_name);

  // clear event maps
  file2contentChange_map.erase(arg->file_name);
  file2lockChange_map.erase(arg->file_name);
  
  return res;
}

std::unique_ptr<RetContentsAndStat>
api_v1_server::getContentsAndStat(std::unique_ptr<FileHandler> arg,
                                  xdr::SessionId session_id, uint32_t xid)
{
  std::unique_ptr<RetContentsAndStat> res(new RetContentsAndStat);
  
  FileHandler *fd = findFd(session_id, *arg);
  if(fd == nullptr) {
    // No match FD found
    res->discriminant(1);
    res->errCode() = BAD_ARG;
    chubby_server_->reply(session_id, xid, std::move(res));
    return res;
  }

  std::string content;
  MetaData meta;
  
  try {
    // try to read in the database
    if(!db.checkAndRead(fd->file_name, fd->instance_number, &content, &meta)) {
      // Read failed
      res->discriminant(1);
      res->errCode() = FS_FAIL;
      chubby_server_->reply(session_id, xid, std::move(res));
      return res;
    }
  } catch (std::exception &e) {
    std::cerr<<"\ngetContentsAndStat() catch an expection: "<<e.what()<<endl;
    res->discriminant(1);
    res->errCode() = PAXOS_ERROR;
    chubby_server_->reply(session_id, xid, std::move(res));
    return res;
  }

  // return normally with ContentsAndStat
  res->discriminant(0);
  res->val().content = content;
  res->val().stat = meta;
  chubby_server_->reply(session_id, xid, std::move(res));

  return res;
}

std::unique_ptr<RetBool>
api_v1_server::setContents(std::unique_ptr<ArgSetContents> arg,
                           xdr::SessionId session_id, uint32_t xid)
{
  std::unique_ptr<RetBool> res(new RetBool);
  
  FileHandler *fd = findFd(session_id, arg->fd);
  if(fd == nullptr) {
    // No match FD found
    res->discriminant(1);
    res->errCode() = BAD_ARG;
    chubby_server_->reply(session_id, xid, std::move(res));
    return res;
  }

  try {
    // try to update in the database
    if(!db.checkAndUpdate(fd->file_name, fd->instance_number, arg->content)) {
      // Update failed
      res->discriminant(1);
      res->errCode() = FS_FAIL;
      chubby_server_->reply(session_id, xid, std::move(res));
      return res;
    }
  } catch (std::exception &e) {
    std::cerr<<"\nsetContents() catch an expection: "<<e.what()<<endl;
    res->discriminant(1);
    res->errCode() = PAXOS_ERROR;
    chubby_server_->reply(session_id, xid, std::move(res));
    return res;
  }
  
  // return normally with TRUE value
  res->discriminant(0);
  res->val() = true;
  chubby_server_->reply(session_id, xid, std::move(res));
  
  // send content change events for current node
  sendContentChangeEvent(fd->file_name);
  
  return res;
}

std::unique_ptr<RetBool>
api_v1_server::acquire(std::unique_ptr<FileHandler> arg,
                       xdr::SessionId session_id, uint32_t xid)
{
  std::unique_ptr<RetBool> res(new RetBool);
  std::string client_id = session2client_map[session_id];
  assert(!client_id.empty());

  cout<<"\nserver: acquire: ("<< arg->file_name << ", "
      << arg->instance_number<<", "<<session_id<<")"<<endl;
  
  FileHandler *fd = findFd(session_id, *arg);
  if(fd == nullptr) {
    // No match FD found
    // return an error
    res->discriminant(1);
    res->errCode() = BAD_ARG;
    chubby_server_->reply(session_id, xid, std::move(res));
    return res;
  }

  try {
    // try to set the lock_owner in the database
    if(db.testAndSetLockOwner(fd->file_name, fd->instance_number, client_id)) {
      // succeeded in DB, return true
      assert(file2lockQueue_map.count(fd->file_name) == 0);
      client2heldLock_map[client_id].insert(fd->file_name);
      res->discriminant(0);
      res->val() = true;
      chubby_server_->reply(session_id, xid, std::move(res));
      printFd();

      // send lock change events for current node
      sendLockChangeEvent(fd->file_name);
      return res;
    }
  } catch (std::exception &e) {
    std::cerr<<"\nacquire() catch an expection: "<<e.what()<<endl;
    res->discriminant(1);
    res->errCode() = PAXOS_ERROR;
    chubby_server_->reply(session_id, xid, std::move(res));
    return res;
  }

  // otherwise, add session information in file2lockQueue_map
  file2lockQueue_map[fd->file_name].push_back({session_id, xid});
  printFd();
  return res;
}

std::unique_ptr<RetBool>
api_v1_server::tryAcquire(std::unique_ptr<FileHandler> arg,
                          xdr::SessionId session_id, uint32_t xid)
{
  std::unique_ptr<RetBool> res(new RetBool);
  std::string client_id = session2client_map[session_id];
  assert(!client_id.empty());

  cout<<"\nserver: tryAcquire: ("<< arg->file_name << ", "
      << arg->instance_number<<", "<<session_id<<")"<<endl;

  FileHandler *fd = findFd(session_id, *arg);
  if(fd == nullptr) {
    // No match FD found
    // return an error
    res->discriminant(1);
    res->errCode() = BAD_ARG;
    chubby_server_->reply(session_id, xid, std::move(res));
    return res;
  }

  try {
    // try to set the lock_owner in the database
    if(db.testAndSetLockOwner(fd->file_name, fd->instance_number, client_id)) {
      // succeeded in DB, return true
      assert(file2lockQueue_map.count(fd->file_name) == 0);
      client2heldLock_map[client_id].insert(fd->file_name);
      res->discriminant(0);
      res->val() = true;
      chubby_server_->reply(session_id, xid, std::move(res));
      printFd();

      // send lock change events for current node
      sendLockChangeEvent(fd->file_name);
      return res;
    }
  } catch (std::exception &e) {
    std::cerr<<"\ntryAcquire() catch an expection: "<<e.what()<<endl;
    res->discriminant(1);
    res->errCode() = PAXOS_ERROR;
    chubby_server_->reply(session_id, xid, std::move(res));
    return res;
  }

  // return false otherwise
  res->discriminant(0);
  res->val() = false;
  chubby_server_->reply(session_id, xid, std::move(res));
  printFd();
  return res;
}

std::unique_ptr<RetBool>
api_v1_server::release(std::unique_ptr<FileHandler> arg,
                       xdr::SessionId session_id, uint32_t xid)
{
  std::unique_ptr<RetBool> res(new RetBool);
  std::string client_id = session2client_map[session_id];
  assert(!client_id.empty());

  cout<<"\nserver: release: ("<< arg->file_name << ", "
      << arg->instance_number<<", "<<session_id<<")"<<endl;

  FileHandler *fd = findFd(session_id, *arg);
  if(fd == nullptr) {
    // No match FD found
    // return an error
    res->discriminant(1);
    res->errCode() = BAD_ARG;
    chubby_server_->reply(session_id, xid, std::move(res));
    return res;
  }

  try {
    // TODO use PAXOS only once
    if(!db.resetLockOwner(fd->file_name, fd->instance_number)) {
      // reset failed
      res->discriminant(1);
      res->errCode() = FS_FAIL;
      chubby_server_->reply(session_id, xid, std::move(res));
      return res;
    }
  } catch (std::exception &e) {
    std::cerr<<"\nrelease() catch an expection: "<<e.what()<<endl;
    res->discriminant(1);
    res->errCode() = PAXOS_ERROR;
    chubby_server_->reply(session_id, xid, std::move(res));
    return res;
  }

  // reset successed
  client2heldLock_map[client_id].erase(fd->file_name);
  if(client2heldLock_map[client_id].empty())
    client2heldLock_map.erase(client_id);
  res->discriminant(0);
  res->val() = true;
  chubby_server_->reply(session_id, xid, std::move(res));
  
  // check the file2lockQueue_map
  auto it = file2lockQueue_map.find(fd->file_name);
  if(it != file2lockQueue_map.end()) {
    auto &lock_queue = it->second;
    assert(lock_queue.size() != 0);
    
    // pop the first element in the queue
    std::pair <xdr::SessionId, uint32_t> acquire_rpc = lock_queue.front();
    lock_queue.pop_front();
    
    // reply acquire() PRC
    std::unique_ptr<RetBool> r(new RetBool);
    std::string client_id_rpc = session2client_map[acquire_rpc.first];
    assert(!client_id_rpc.empty());
      
    client2heldLock_map[client_id_rpc].insert(fd->file_name);
    r->discriminant(0);
    r->val() = true;
    chubby_server_->reply(acquire_rpc.first, acquire_rpc.second, std::move(r));
        
    // garbage collect
    if(lock_queue.empty())
      file2lockQueue_map.erase(it);
    printFd();
  }

  // send lock change events for current node
  sendLockChangeEvent(fd->file_name);
  return res;
}


std::unique_ptr<RetBool>
api_v1_server::startSession(std::unique_ptr<longstring> arg,
			    xdr::SessionId session_id, uint32_t xid)
{
  std::unique_ptr<RetBool> res(new RetBool);
  
  try {
    // TODO test itself whether it is a leader
    session2client_map[session_id] = *arg;

    res->discriminant(0);
    res->val() = true;
  } catch (std::exception &e) {
    std::cerr<<"startSession catch an expection: "<<e.what()<<endl;
    res->discriminant(0);
    res->val() = false;
  }
  chubby_server_->reply(session_id, xid, std::move(res));
  return res;
}

std::unique_ptr<RetBool>
api_v1_server::fileReopen(std::unique_ptr<ArgReopen> arg,
			  xdr::SessionId session_id, uint32_t xid)
{
  std::unique_ptr<RetBool> res(new RetBool);
  FileHandler fd = arg->fd;
  Mode mode = arg->mode;
  
  // TODO check validality of FD
  FileHandler *new_fd = new FileHandler();
  *new_fd = fd;
  // add FD to <file, list of (session, FD) pairs> 
  file2fd_map[fd.file_name].push_back({session_id, new_fd});
  // add FD to <session, list of FDs> map
  session2fd_map[session_id].push_back(new_fd);

  // register events
  if (mode & EV_LOCK_CHANGED)
    file2lockChange_map[fd.file_name].push_back(session_id);
  if (mode & EV_CONTENT_MODIFIED)
    file2contentChange_map[fd.file_name].push_back(session_id);
  
  res->discriminant(0);
  res->val() = true;
  chubby_server_->reply(session_id, xid, std::move(res));
  return res;
}

/* This method is called by CHUBBY_SERVER_ when it detects a time-out session.
 * This method will effectively close all the file handlers of the session,
 * and reclaim all the locks owned by the session.*/
void
api_v1_server::disconnect(xdr::SessionId session_id)
{
  auto openedFds = session2fd_map[session_id]; // make a copy first
  for(FileHandler *fd : openedFds) {
    std::unique_ptr<FileHandler> arg(new FileHandler);
    *arg = *fd;
    fileClose(std::move(arg), session_id, (uint32_t) -1);
  }
  session2client_map.erase(session_id);
}

void
api_v1_server::initializeLeader()
{
  session2client_map.clear();
  file2fd_map.clear();
  session2fd_map.clear();
  file2lockQueue_map.clear();
  client2heldLock_map.clear();
  client2ephemeral_map.clear();
  file2lockChange_map.clear();
  file2contentChange_map.clear();

  // recover from persistent data store
  std::vector<std::pair<std::string, std::string> > client2heldLock_list;
  db.getStates(client2heldLock_list);
  for (auto& p : client2heldLock_list)
    client2heldLock_map[p.first].insert(p.second);
}


/* Returns true is C is any letter, numbers, underscore, or slash. */
inline bool
checkChar(char c)
{
  std::string validChars = "_/0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
  return validChars.find(c) != std::string::npos;
}

/* Paths must begin with a '/' and must contain only letters, numbers, 
   underscores, and single slashes to separate components. 
   Cannot be "/". Cannot end with '/'.*/
bool 
checkName(const std::string &key)
{
  // key must begin with '/'
  if (key[0] != '/') return false;
  // path cannot end "/"
  if (key[key.length() - 1] == '/') return false;
  // no space
  for (int i = 0; i < key.length(); ++i)
    {
      // paths must contain only letters, numbers, underscores, and slashes to separate components
      if(!checkChar(key[i])) return false;
    }
  // path cannot contain "//"
  if(key.find("//") != std::string::npos) return false;
  return true;
}


FileHandler *
api_v1_server::findFd(xdr::SessionId session_id, const FileHandler &fd)
{
  std::list<FileHandler *> l = session2fd_map[session_id];
    
  for(auto it = l.begin(); it != l.end(); ++it) {
    FileHandler *p = *it;
    if (p->instance_number == fd.instance_number &&
	p->magic_number == fd.magic_number &&
	p->master_sequence_number == fd.master_sequence_number &&
	p->file_name.compare(fd.file_name) == 0 &&
	p->write_is_allowed == fd.write_is_allowed)
      return p;
  }
  // TODO check validality of FD
  FileHandler *new_fd = new FileHandler();
  *new_fd = fd;
  // add FD to <file, list of (session, FD) pairs> 
  file2fd_map[fd.file_name].push_back({session_id, new_fd});
  // add FD to <session, list of FDs> map
  session2fd_map[session_id].push_back(new_fd);
  return new_fd;

  // return nullptr if no match found
  return nullptr;
}

void
api_v1_server::sendLockChangeEvent(const std::string &file_name)
{
  if (file2lockChange_map.count(file_name) > 0) {
    EventContent evc;
    evc.event = ChubbyEvent::LOCK_CHANGED;
    evc.fname = file_name;
    for (auto s : file2lockChange_map[file_name])
      chubby_server_->send<event_interface::event_callback_t> (s, evc);
    // delete the list
    //file2lockChange_map.erase(file_name);
  }
}

void
api_v1_server::sendContentChangeEvent(const std::string &file_name)
{
  if (file2contentChange_map.count(file_name) > 0) {
    EventContent evc;
    evc.event = ChubbyEvent::CONTENT_MODIFIED;
    evc.fname = file_name;
    for (auto s : file2contentChange_map[file_name])
      chubby_server_->send<event_interface::event_callback_t> (s, evc);
    // delete the list
    //file2contentChange_map.erase(file_name);
  }
}
