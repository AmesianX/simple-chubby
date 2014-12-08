#include <exception>

#include "paxos/helper.hh"
#include "paxos/paxos_lib.hh"
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
    const std::string &file_name, bool is_dir, uint64_t instance_number) {
  std::unique_ptr<execute_arg> arg(new execute_arg);
  // arg->request.append((const unsigned char*)line.c_str(), line.size());
  std::unique_ptr<execute_res> result =
      paxos_lib_->paxos_interface_for_user->execute(std::move(arg));
  if (result->ok()) {
    std::string reply_str = OpaqueToString(result->reply());
    std::cout << "Reply: " << reply_str << std::endl;
    return true;
  } else {
    std::cout << "Not the leader." << std::endl;
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
// TODO: can be deleted.
std::string ServerDBPaxos::getParentName(const std::string &key) {
  return std::string();
}
