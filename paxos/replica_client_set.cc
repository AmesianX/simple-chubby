#include "paxos/replica_client_set.hh"

std::pair<std::string, std::string> ReplicaClientSet::analyzeNetworkAddress(
    const net_address_t& network_address) {
  std::string::size_type colon_pos = network_address.find(":");
  assert(colon_pos != std::string::npos);
  return std::make_pair(network_address.substr(0, colon_pos),
                        network_address.substr(colon_pos + 1));
}

ReplicaClientSet::ReplicaClientSet(std::vector<net_address_t> other_replicas) {
  pthread_mutex_init(&start_connecting_lock_, nullptr);
  replica_client_list_.resize(other_replicas.size());
  for (int i = 0; i < other_replicas.size(); ++i) {
    replica_client_list_[i].network_address = other_replicas[i];
  }
}

ReplicaClientSet::~ReplicaClientSet() {
  pthread_mutex_destroy(&start_connecting_lock_);
}

void ReplicaClientSet::detectFailure(
    ReplicaClientSet::ReplicaClientState* replica_client) {
  struct pollfd poll_fd;
  poll_fd.fd = replica_client->fd.get();
  poll_fd.events = POLLHUP;
  poll_fd.revents = 0;
  // Poll until the other side hangs up the link.
  poll(&poll_fd, 1, -1);
  pthread_mutex_lock(&start_connecting_lock_);
  pthread_mutex_lock(&replica_client->lock);
  std::cerr << "Link to " << replica_client->network_address
      << " is hung up." << std::endl;
  replica_client->fd.release();
  delete replica_client->replica_client;
  replica_client->replica_client = nullptr;
  pthread_mutex_unlock(&replica_client->lock);
  pthread_mutex_unlock(&start_connecting_lock_);
}

void ReplicaClientSet::tryConnect() {
  pthread_mutex_lock(&start_connecting_lock_);
  for (int i = 0; i < replica_client_list_.size(); ++i) {
    auto& state = replica_client_list_[i];
    if (!state.replica_client) {
      auto host_and_port = analyzeNetworkAddress(state.network_address);
      try {
        state.fd = xdr::tcp_connect(std::get<0>(host_and_port).c_str(),
                                    std::get<1>(host_and_port).c_str());
        xdr::set_close_on_exec(state.fd.get());
        state.replica_client = new ReplicaClientType(state.fd.get());
        std::thread detect_thread(std::bind(
                &ReplicaClientSet::detectFailure, this, &state));
        detect_thread.detach();
      } catch (std::system_error& err) {
        state.replica_client = nullptr;
        // Silence, connection fails, try again later.
      }
    }
  }
  pthread_mutex_unlock(&start_connecting_lock_);
}

ReplicaClientSet::ReplicaClientType* ReplicaClientSet::getReplicaClient(
    int rank_id) {
  tryConnect();
  pthread_mutex_lock(&start_connecting_lock_);
  auto& state = replica_client_list_[rank_id];
  pthread_mutex_lock(&state.lock);
  if (state.replica_client) {
    pthread_mutex_unlock(&start_connecting_lock_);
    return state.replica_client;
  } else {
    pthread_mutex_unlock(&start_connecting_lock_);
    return nullptr;
  }
}
void ReplicaClientSet::releaseReplicaClient(int rank_id) {
  pthread_mutex_unlock(&replica_client_list_[rank_id].lock);
}
