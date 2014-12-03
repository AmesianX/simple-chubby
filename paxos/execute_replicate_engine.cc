#include "paxos/replica_client_set.hh"
#include "paxos/replica_state.hh"

#include "paxos/execute_replicate_engine.hh"

void ExecuteReplicateEngine::replicateCommand(
    const init_view_request& request) {
  replicate_arg command;
  xdr::msg_ptr message = xdr::xdr_to_msg(request);
  command.arg.request.append(
      (const unsigned char*)message->raw_data(), message->raw_size());
  for (int i = 0; i < replica_state_->getQuota(); ++i) {
    if (i != replica_state_->getSelfRank()) {
      auto* replica_client = replica_client_set_->getReplicaClient(i);
      if (replica_client) {
        // Result is ignored.
        replica_client->replicate(command);
        replica_client_set_->releaseReplicaClient(i);
      }
    }
  }
}
