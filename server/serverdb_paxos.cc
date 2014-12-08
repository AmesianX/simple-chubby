#include <exception>

#include "xdrpp/message.h"
#include "xdrpp/marshal.h"
#include "paxos/helper.hh"
#include "paxos/paxos_lib.hh"
#include "include/server.hh"

#include "server/serverdb_paxos.hh"


ServerDBPaxos::ServerDBPaxos(PaxosLib* paxos_lib) {
  paxos_lib_ = paxos_lib;
}
ServerDBPaxos::~ServerDBPaxos() {
}
//struct execute_arg {
//  cid_t cid;
//  uid_t rid;
//  viewid_t vid;
//  opaque request<>;
//};
//union execute_res switch (bool ok) {
//  case TRUE:
//    opaque reply<>;
//  case FALSE:
//    execute_viewinfo viewinfo;
//};

ServerPaxosResult
ServerDBPaxos::runPaxos(const ServerPaxosParam &param)
{
  std::unique_ptr<execute_arg> arg(new execute_arg);
  // Serialize param to arg->request().
  xdr::msg_ptr serialized_param = xdr::xdr_to_msg(param);
  arg->request.append(
      (const unsigned char*)serialized_param->data(),
      serialized_param->size());

  // Call paxos.
  std::unique_ptr<execute_res> result =
      paxos_lib_->paxos_interface_for_user->execute(std::move(arg));
  if (result->ok()) {
    // Deserialize result->reply() to reply_result.
    xdr::msg_ptr serialized_result =
        xdr::message_t::alloc(result->reply().size());
    memcpy(serialized_result->data(), result->reply().data(),
           result->reply().size());
    ServerPaxosResult reply_result;
    xdr_from_msg(serialized_result, reply_result);
    std::cout << "Paxos transaction succeeded." << std::endl;
    return reply_result;
  } else {
    std::cout << "Paxos transaction failed." << std::endl;
    throw std::runtime_error("Not the Paxos leader.");
  }
}


bool ServerDBPaxos::checkAndCreate(
    const std::string &file_name, bool is_dir, uint64_t *instance_number) {
  // Build param.
  ServerPaxosParam param(CHECK_AND_CREATE);
  param.check_and_create_param().file_name = file_name;
  param.check_and_create_param().is_dir = is_dir;
  
  ServerPaxosResult reply_result = runPaxos(param);

  // Examine reply_result.
  *instance_number = reply_result.check_and_create_result().instance_number;
  std::cout << "return:" << *instance_number
	    << " " << reply_result.check_and_create_result().success
	    << std::endl;
  return reply_result.check_and_open_result().success;
}

bool ServerDBPaxos::checkAndOpen(
    const std::string &file_name, uint64_t *instance_number) {

  ServerPaxosParam param(CHECK_AND_OPEN);
  param.check_and_open_param().file_name = file_name;
  
  ServerPaxosResult reply_result = runPaxos(param);

  // Examine reply_result.
  *instance_number = reply_result.check_and_open_result().instance_number;
  std::cout << "return:" << *instance_number
	    << " " << reply_result.check_and_open_result().success
	    << std::endl;
  return reply_result.check_and_open_result().success;
}

bool ServerDBPaxos::checkAndDelete(
    const std::string &file_name, uint64_t instance_number) {

  ServerPaxosParam param(CHECK_AND_DELETE);
  param.check_and_delete_param().file_name = file_name;
  param.check_and_delete_param().instance_number = instance_number;
  
  ServerPaxosResult reply_result = runPaxos(param);

  // Examine reply_result.
  std::cout << "return:" << reply_result.check_and_delete_result().success
	    << std::endl;
  return reply_result.check_and_delete_result().success;
}

bool ServerDBPaxos::checkAndRead(
    const std::string &file_name, uint64_t instance_number,
    std::string *content, MetaData *meta) {

  ServerPaxosParam param(CHECK_AND_READ);
  param.check_and_read_param().file_name = file_name;
  param.check_and_read_param().instance_number = instance_number;
  
  ServerPaxosResult reply_result = runPaxos(param);

  // Examine reply_result.
  *content = reply_result.check_and_read_result().content;
  meta->instance_number = reply_result.check_and_read_result().meta.instance_number;
  meta->content_generation_number = reply_result.check_and_read_result().meta.content_generation_number;
  meta->lock_generation_number = reply_result.check_and_read_result().meta.lock_generation_number;
  meta->file_content_checksum = reply_result.check_and_read_result().meta.file_content_checksum;
  meta->is_directory = reply_result.check_and_read_result().meta.is_directory;

  std::cout << "return:" << *content
	    << " " << reply_result.check_and_read_result().success
	    << std::endl;
  return reply_result.check_and_read_result().success;
}
bool ServerDBPaxos::checkAndUpdate(
    const std::string &file_name, uint64_t instance_number,
    const std::string &content) {

  ServerPaxosParam param(CHECK_AND_UPDATE);
  param.check_and_update_param().file_name = file_name;
  param.check_and_update_param().instance_number = instance_number;
  param.check_and_update_param().content = content;
   
  ServerPaxosResult reply_result = runPaxos(param);

  // Examine reply_result.
  std::cout << "return:" << reply_result.check_and_update_result().success
	    << std::endl;
  return reply_result.check_and_update_result().success;
}
bool ServerDBPaxos::testAndSetLockOwner(
    const std::string &file_name, uint64_t instance_number,
    const std::string &client_id) {
  
  ServerPaxosParam param(TEST_AND_SET_LOCK_OWNER);
  param.test_and_set_lock_owner_param().file_name = file_name;
  param.test_and_set_lock_owner_param().instance_number = instance_number;
  param.test_and_set_lock_owner_param().client_id = client_id;
  
  ServerPaxosResult reply_result = runPaxos(param);

  // Examine reply_result.
  std::cout << "return:" << reply_result.test_and_set_lock_owner_result().success
	    << std::endl;
  return reply_result.test_and_set_lock_owner_result().success;
return true;
}
bool ServerDBPaxos::resetLockOwner(
    const std::string &file_name, uint64_t instance_number) {

  ServerPaxosParam param(RESET_LOCK_OWNER);
  param.reset_lock_owner_param().file_name = file_name;
  param.reset_lock_owner_param().instance_number = instance_number;
  
  ServerPaxosResult reply_result = runPaxos(param);

  // Examine reply_result.
  std::cout << "return:" << reply_result.reset_lock_owner_result().success
	    << std::endl;
  return reply_result.reset_lock_owner_result().success;
return true;
}
void ServerDBPaxos::getStates(
    std::vector<std::pair<std::string, std::string> > &client2heldLock) {
  ServerPaxosParam param(GET_STATES);
  
  ServerPaxosResult reply_result = runPaxos(param);

  // Examine reply_result.
  auto list = reply_result.get_states_result().list;
  for(auto & e : list)
    client2heldLock.push_back({e.key, e.value});
  std::cout << "return: size of the returned list = " << list.size()
	    << std::endl;
  
  return;
}
