#include <cassert>
#include <iostream>
#include <functional>
#include "xdrpp/marshal.h"
#include "xdrpp/message.h"
#include "server/server_paxos.hh"
#include "server/serverdb_backstore.hh"
std::string ServerdbBackstore::Run(const std::string input) {
  std::cout << "[paxos backstore] accepted: " << input << std::endl;

  // Deserialize input to param.
  ServerPaxosParam param;
  xdr::msg_ptr serialized_param = xdr::message_t::alloc(input.size());
  memcpy(serialized_param->data(), input.data(), input.size());
  xdr::xdr_from_msg(serialized_param, param);

  // Build result.
  ServerPaxosResult result(CHECK_AND_CREATE);
  std::string file_name;
  bool is_dir;
  uint64_t instance_number;
  bool success;
  std::string content;
  MetaData meta;
  std::string client_id;
  std::vector<std::pair<std::string, std::string> >
      client2heldLock;
  switch (param.type()) {
    case CHECK_AND_CREATE:
      file_name = param.check_and_create_param().file_name;
      is_dir = param.check_and_create_param().is_dir;
      std::cout << "[paxos backstore] analyzed: " << file_name
          << " " << is_dir << std::endl;
      success = server_db.checkAndCreate(file_name, is_dir, &instance_number);
      result.type(CHECK_AND_CREATE);
      result.check_and_create_result().instance_number = instance_number;
      result.check_and_create_result().success = success;
      break;
    case CHECK_AND_OPEN:
      file_name = param.check_and_open_param().file_name;
      std::cout << "[paxos backstore] analyzed: " << file_name << std::endl;
      success = server_db.checkAndOpen(file_name, &instance_number);
      result.type(CHECK_AND_OPEN);
      result.check_and_open_result().instance_number = instance_number;
      result.check_and_open_result().success = success;
      break;
    case CHECK_AND_DELETE:
      file_name = param.check_and_delete_param().file_name;
      instance_number = param.check_and_delete_param().instance_number;
      std::cout << "[paxos backstore] analyzed: " << file_name << " "
          << instance_number <<std::endl;
      success = server_db.checkAndDelete(file_name, instance_number);
      result.type(CHECK_AND_DELETE);
      result.check_and_delete_result().success = success;
      break;
    case CHECK_AND_READ:
      file_name = param.check_and_read_param().file_name;
      instance_number = param.check_and_read_param().instance_number;
      std::cout << "[paxos backstore] analyzed: " << file_name << " "
          << instance_number <<std::endl;
      success = server_db.checkAndRead(
          file_name, instance_number, &content, &meta);
      result.type(CHECK_AND_READ);
      result.check_and_read_result().success = success;
      result.check_and_read_result().content = content;
      result.check_and_read_result().meta.instance_number
          = meta.instance_number;
      result.check_and_read_result().meta.content_generation_number
          = meta.content_generation_number;
      result.check_and_read_result().meta.lock_generation_number
          = meta.lock_generation_number;
      result.check_and_read_result().meta.file_content_checksum
          = meta.file_content_checksum;
      result.check_and_read_result().meta.is_directory
          = meta.is_directory;
      break;
    case CHECK_AND_UPDATE:
      file_name = param.check_and_update_param().file_name;
      instance_number = param.check_and_update_param().instance_number;
      content = param.check_and_update_param().content;
      std::cout << "[paxos backstore] analyzed: " << file_name << " "
          << instance_number <<std::endl;
      success = server_db.checkAndUpdate(
          file_name, instance_number, content);
      result.type(CHECK_AND_UPDATE);
      result.check_and_update_result().success = success;
      break;
    case TEST_AND_SET_LOCK_OWNER:
      file_name = param.test_and_set_lock_owner_param().file_name;
      instance_number = param.test_and_set_lock_owner_param().instance_number;
      client_id = param.test_and_set_lock_owner_param().client_id;
      std::cout << "[paxos backstore] analyzed: " << file_name << " "
          << instance_number << " " << client_id << std::endl;
      success = server_db.testAndSetLockOwner(
          file_name, instance_number, client_id);
      result.type(TEST_AND_SET_LOCK_OWNER);
      result.test_and_set_lock_owner_result().success = success;
      break;
    case RESET_LOCK_OWNER:
      file_name = param.reset_lock_owner_param().file_name;
      instance_number = param.reset_lock_owner_param().instance_number;
      std::cout << "[paxos backstore] analyzed: " << file_name << " "
          << instance_number <<std::endl;
      success = server_db.resetLockOwner(
          file_name, instance_number);
      result.type(RESET_LOCK_OWNER);
      result.reset_lock_owner_result().success = success;
      break;
    case GET_STATES:
      server_db.getStates(client2heldLock);
      result.type(GET_STATES);
      // result.get_states_result().list = translate(client2lock);
      server_db.getStates(client2heldLock);
      for (auto& item : client2heldLock) {
        key_value temp;
        temp.key = item.first;
        temp.value = item.second;
        result.get_states_result().list.push_back(temp);
      }
      break;
    default:
      assert(false);
  }  // End of switch.


  // Serialize result to serialized_result.
  xdr::msg_ptr serialized_result = xdr::xdr_to_msg(result);
  return std::string(serialized_result->data(),
                     serialized_result->size());
}
