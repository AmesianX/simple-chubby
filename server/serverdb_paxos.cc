#include <exception>

#include "xdrpp/message.h"
#include "xdrpp/marshal.h"
#include "paxos/helper.hh"
#include "paxos/paxos_lib.hh"
#include "server/server_paxos.hh"
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
bool ServerDBPaxos::checkAndCreate(
    const std::string &file_name, bool is_dir, uint64_t *instance_number) {
  std::unique_ptr<execute_arg> arg(new execute_arg);

  // Build param.
  ServerPaxosParam param(CHECK_AND_CREATE);
  param.check_and_create_param().file_name = file_name;
  param.check_and_create_param().is_dir = is_dir;

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
    // Examine reply_result.
    *instance_number = reply_result.check_and_create_result().instance_number;
    std::cout << "return:" << *instance_number
        << " " << reply_result.check_and_create_result().success
        << std::endl;
    std::cout << "Paxos transaction succeeded." << std::endl;
    return reply_result.check_and_create_result().success;
  } else {
    std::cout << "Paxos transaction failed." << std::endl;
    throw std::runtime_error("Not the Paxos leader.");
  }
}

bool ServerDBPaxos::checkAndOpen(
    const std::string &file_name, uint64_t *instance_number) {
  return true;
}
bool ServerDBPaxos::checkAndDelete(
    const std::string &file_name, uint64_t instance_number) {
  return true;
}
bool ServerDBPaxos::checkAndRead(
    const std::string &file_name, uint64_t instance_number,
    std::string *content, MetaData *meta) {
  return true;
}
bool ServerDBPaxos::checkAndUpdate(
    const std::string &file_name, uint64_t instance_number,
    const std::string &content) {
  return true;
}
bool ServerDBPaxos::testAndSetLockOwner(
    const std::string &file_name, uint64_t instance_number,
    const std::string &client_id) {
  return true;
}
bool ServerDBPaxos::resetLockOwner(
    const std::string &file_name, uint64_t instance_number) {
  return true;
}
void ServerDBPaxos::getStates(
    std::vector<std::pair<std::string, std::string> > &client2heldLock) {
}
