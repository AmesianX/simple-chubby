// Scaffolding originally generated from include/server.x.
// Edit to add functionality.

#include "server/serverimpl.hh"
#include <cstdint>
#include <cassert>

const uint64_t READ = 0x1;
const uint64_t WRITE = 0x2;
const uint64_t CREATE_DIRECTORY = 0x4;
const uint64_t CREATE_FILE = 0x8;

std::unique_ptr<RetFd>
api_v1_server::fileOpen(std::unique_ptr<ArgOpen> arg)
{
  std::unique_ptr<RetFd> res(new RetFd);
  std::string file_name = arg->name;
  Mode mode = arg->mode;
  uint64_t client_id = 123; // TODO

  if(!checkName(file_name)) {
      // TODO return with error
  }   
  
  if((mode & CREATE_DIRECTORY) && (mode & CREATE_DIRECTORY)) {
      // TODO return with error
  }

  // check if client has already opened this file
  auto l = client2fd_map[client_id];
  for (auto it = l.begin(); it != l.end(); ++it)
      if ((*it)->file_name.compare(file_name) == 0) {
	  // FD with the same file_name exists
	  // TODO return error
      }

  FileHandler *fd = new FileHandler();
  // Create file or dir
  if((mode & CREATE_DIRECTORY) || (mode & CREATE_DIRECTORY)) {
      bool is_dir = mode & CREATE_DIRECTORY;
      ++this->instance_number;
      // check file doesn't exist and parent exits
      if(!db.checkAndCreate(file_name, is_dir, this->instance_number)) {
	  // TODO creation failed, then return false

      }
      fd->instance_number = this->instance_number;
  } else { // open an existing file or dir
      // check file is exist
      uint64_t instance_number;
      if(!db.checkAndOpen(file_name, &instance_number)) {
	  // TODO open failed, then return false

      }
      fd->instance_number = instance_number;
  }
  fd->magic_number = this->rand_gen();
  fd->master_sequence_number = this->master_sequence_number;
  fd->file_name = file_name;
  fd->write_is_allowed = mode & WRITE;

  // add FD to <file, list of (client, FD) pairs> map
  //std::map<std::string, std::list<ClientFdPair> > file2fd_map;
  ClientFdPair pair;
  pair.client = client_id;
  pair.fd = fd;
  file2fd_map[file_name].push_back(pair);

  // add FD to <client, list of FDs> map
  // std::map<uint64_t, std::list<FileHandler* > > client2fd_map;
  client2fd_map[client_id].push_back(fd);
     
  // return normally with FD
  res->discriminant(0);
  res->val() = *fd;
  return res;
}

std::unique_ptr<RetBool>
api_v1_server::fileClose(std::unique_ptr<FileHandler> arg)
{
  std::unique_ptr<RetBool> res(new RetBool);
  uint64_t client_id = 123; // TODO
  
  FileHandler *fd = findFd(client_id, *arg);
  if(fd == nullptr) {
      // No match FD found
      // return normally with TRUE value
      res->discriminant(0);
      res->val() = true;
      return res;
  }

  ClientFdPair pair {client_id, fd};
  // remove FD from <file, list of (client, FD) pairs> map
  file2fd_map[fd->file_name].remove(pair);

  // remove FD from <client, list of FDs> map
  client2fd_map[client_id].remove(fd);

  // delete FD
  delete fd;
  
  // return normally with TRUE value
  res->discriminant(0);
  res->val() = true;
  return res;
}

std::unique_ptr<RetBool>
api_v1_server::fileDelete(std::unique_ptr<FileHandler> arg)
{
  std::unique_ptr<RetBool> res(new RetBool);
  uint64_t client_id = 123; // TODO
  
  FileHandler *fd = findFd(client_id, *arg);
  if(fd == nullptr) {
      // No match FD found
      // TODO throw an exception
      return res;
  }

  // try to delete in the database
  if(!db.checkAndDelete(fd->file_name, fd->instance_number)) {
      // Delete failed, return false
      res->discriminant(0);
      res->val() = false;
      return res;
  }

  // remove all FDs in file2fd_map[fd->file_name]
  std::list<ClientFdPair> l = file2fd_map[fd->file_name];
  for (auto it = l.begin(); it != l.end(); ++it) {
      uint64_t c = (*it).client;
      FileHandler *f = (*it).fd;
      // remove this FD in client2fd_map
      client2fd_map[c].remove(f);
      // free space
      delete f;
  }  
  // remove the list in file2fd_map
  size_t r = file2fd_map.erase(fd->file_name);
  assert(r == 1);
  
  // return normally with TRUE value
  res->discriminant(0);
  res->val() = true;
  return res;
}

std::unique_ptr<RetContentsAndStat>
api_v1_server::getContentsAndStat(std::unique_ptr<FileHandler> arg)
{
  std::unique_ptr<RetContentsAndStat> res(new RetContentsAndStat);
  uint64_t client_id = 123; // TODO
  
  FileHandler *fd = findFd(client_id, *arg);
  if(fd == nullptr) {
      // No match FD found
      // TODO throw an exception
      return res;
  }

  std::string content;
  MetaData meta;
  // try to read in the database
  if(!db.checkAndRead(fd->file_name, fd->instance_number, &content, &meta)) {
      // Read failed
      // TODO throw an exception
      return res;
  }

  // return normally with ContentsAndStat
  res->discriminant(0);
  res->val().content = content;
  res->val().stat = meta;
  return res;
}

std::unique_ptr<RetBool>
api_v1_server::setContents(std::unique_ptr<ArgSetContents> arg)
{
  std::unique_ptr<RetBool> res(new RetBool);
  uint64_t client_id = 123; // TODO
  
  FileHandler *fd = findFd(client_id, arg->fd);
  if(fd == nullptr) {
      // No match FD found
      // TODO throw an exception
      return res;
  }

  // try to update in the database
  if(!db.checkAndUpdate(fd->file_name, fd->instance_number, arg->content)) {
      // Update failed
      // TODO throw an exception
      return res;
  }
  
  // return normally with TRUE value
  res->discriminant(0);
  res->val() = true;
  return res;
}

std::unique_ptr<RetBool>
api_v1_server::acquire(std::unique_ptr<FileHandler> arg)
{
  std::unique_ptr<RetBool> res(new RetBool);
  
  // Fill in function body here
  
  return res;
}

std::unique_ptr<RetBool>
api_v1_server::tryAcquire(std::unique_ptr<FileHandler> arg)
{
  std::unique_ptr<RetBool> res(new RetBool);
  
  // Fill in function body here
  
  return res;
}

std::unique_ptr<RetBool>
api_v1_server::release(std::unique_ptr<FileHandler> arg)
{
  std::unique_ptr<RetBool> res(new RetBool);
  
  // Fill in function body here
  
  return res;
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
api_v1_server::checkName(const std::string &key)
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
api_v1_server::findFd(uint64_t client_id, const FileHandler &fd)
{
    std::list<FileHandler *> l = client2fd_map[client_id];
    
    for(auto it = l.begin(); it != l.end(); ++it) {
	FileHandler *p = *it;
	if (p->instance_number == fd.instance_number &&
	    p->magic_number == fd.magic_number &&
	    p->master_sequence_number == fd.master_sequence_number &&
	    p->file_name.compare(fd.file_name) == 0 &&
	    p->write_is_allowed == fd.write_is_allowed)
	    return p;
    }
    // return nullptr if no match found
    return nullptr;
}

#if 0

int
main(int argc, const char *argv[])
{
    api_v1_server s();
}

#endif
