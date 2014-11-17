// Scaffolding originally generated from include/server.x.
// Edit to add functionality.

#include "server/serverimpl.hh"
#include <cstdint>

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
  
  if((mode & CREATE_DIRECTORY) && (mode & CREATE_DIRECTORY)) {
      // TODO return with error
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
  
  // Fill in function body here
  
  return res;
}

std::unique_ptr<RetBool>
api_v1_server::fileDelete(std::unique_ptr<FileHandler> arg)
{
  std::unique_ptr<RetBool> res(new RetBool);
  
  // Fill in function body here
  
  return res;
}

std::unique_ptr<RetContentsAndStat>
api_v1_server::getContentsAndStat(std::unique_ptr<FileHandler> arg)
{
  std::unique_ptr<RetContentsAndStat> res(new RetContentsAndStat);
  
  // Fill in function body here
  
  return res;
}

std::unique_ptr<RetBool>
api_v1_server::setContents(std::unique_ptr<ArgSetContents> arg)
{
  std::unique_ptr<RetBool> res(new RetBool);
  
  // Fill in function body here
  
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
