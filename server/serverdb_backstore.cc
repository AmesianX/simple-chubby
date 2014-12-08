#include <iostream>
#include "xdrpp/marshal.h"
#include "xdrpp/message.h"
#include "server/server_paxos.hh"
#include "server/serverdb_backstore.hh"
std::string ServerdbBackstore::Run(const std::string input) {
  std::cout << "[paxos backstore] accepted: " << input << std::endl;

  // Deserialize input to param.
  ServerPaxosParam param;
  xdr::msg_ptr serialized_param = xdr::message_t::alloc(input.size()-4);
  memcpy(serialized_param->data(), input.data()+4, input.size()-4);
  xdr::xdr_from_msg(serialized_param, param);

  // Read param.
  std::string file_name =
      param.check_and_create_param().file_name;
  bool is_dir = param.check_and_create_param().is_dir;
  std::cout << "[paxos backstore] analyzed: " << file_name
      << " " << is_dir << std::endl;

  // Run serverdb call.
  uint64_t instance_number;
  bool success;
  success = server_db.checkAndCreate(file_name, is_dir, &instance_number);
  instance_number = 1989;

  // Build result.
  ServerPaxosResult result(CHECK_AND_CREATE);
  result.check_and_create_result().instance_number = instance_number;
  result.check_and_create_result().success = success;

  // Serialize result to serialized_result.
  xdr::msg_ptr serialized_result = xdr::xdr_to_msg(result);
  return std::string(serialized_result->raw_data(),
                     serialized_result->raw_size());
}
