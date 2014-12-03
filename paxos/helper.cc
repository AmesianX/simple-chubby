#include "paxos/helper.hh"

std::pair<std::string, std::string> analyzeNetworkAddress(
    const net_address_t& network_address) {
  std::string::size_type colon_pos = network_address.find(":");
  assert(colon_pos != std::string::npos);
  return std::make_pair(network_address.substr(0, colon_pos),
                        network_address.substr(colon_pos + 1));
}

std::string analyzeNetworkPort(
    const net_address_t& network_address) {
  std::string::size_type colon_pos = network_address.find(":");
  assert(colon_pos != std::string::npos);
  return network_address.substr(colon_pos + 1);
}

std::string OpaqueToString(const xdr::opaque_vec<>& input) {
  return std::string((const char*)input.data(), input.size());
}
void OpaqueToString(const xdr::opaque_vec<>& input, std::string* output) {
  output->assign((const char*)input.data(), input.size());
}
void StringToOpaque(const std::string& input, xdr::opaque_vec<>* output) {
  output->clear();
  output->append((const unsigned char*)input.c_str(), input.size());
}
