#include "paxos/paxos_lib.hh"
#include "server/serverdb_paxos.hh"

ServerDBPaxos::ServerDBPaxos(const char *file, PaxosLib* paxos_lib) {
}
ServerDBPaxos::~ServerDBPaxos() {
}
bool ServerDBPaxos::checkAndCreate(
    const std::string &file_name, bool is_dir, uint64_t instance_number) {
  return true;
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
