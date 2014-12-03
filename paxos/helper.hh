#ifndef __PAXOS_HELPER_HH_INCLUDED__
#define __PAXOS_HELPER_HH_INCLUDED__ 1

#include <cassert>
#include <string>
#include <utility>

#include "paxos/paxos.hh"

std::pair<std::string, std::string> analyzeNetworkAddress(
    const net_address_t& network_address);

std::string analyzeNetworkPort(
    const net_address_t& network_address);

std::string OpaqueToString(const xdr::opaque_vec<>& input);

void OpaqueToString(const xdr::opaque_vec<>& input, std::string* output);

void StringToOpaque(const std::string& input, xdr::opaque_vec<>* output);

#endif  // __PAXOS_HELPER_HH_INCLUDED__
