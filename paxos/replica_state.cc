#include "paxos/replica_state.hh"
ReplicaState::ReplicaState(const std::string& path_name, int self_rank) {
  std::ifstream config_file;
  config_file.open(path_name);
  std::string line;
  std::getline(config_file, line);
  quota_ = std::stoi(line);
  for (int i = 0; i < quota_; ++i) {
    std::getline(config_file, line);
    replica_address_.push_back(net_address_t(line));
    std::getline(config_file, line);
    client_use_address_.push_back(net_address_t(line));
  }
  config_file.close();
  pthread_mutex_init(&lock_, nullptr);
}
