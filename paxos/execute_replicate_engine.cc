#include "paxos/replica_client_set.hh"
#include "paxos/replica_state.hh"
#include "paxos/helper.hh"

#include "paxos/execute_replicate_engine.hh"

void ExecuteReplicateEngine::replicateCommand(
    const init_view_request& request) {
  replicate_arg command;
  xdr::msg_ptr message = xdr::xdr_to_msg(request);
  command.arg.request.append(
      (const unsigned char*)message->raw_data(), message->raw_size());
  for (int i = 0; i < replica_state_->getMaxNumClient(); ++i) {
    if (i != replica_state_->getSelfRank()) {
      auto* replica_client = replica_client_set_->getReplicaClient(i);
      if (replica_client) {
        // Result is ignored.
        command.vs.ts = 0;
        replica_client->replicate(command);
        replica_client_set_->releaseReplicaClient(i);
      }
    }
  }
}

std::string ExecuteReplicateEngine::replicateCommand(
    const std::string& request) {
  replicate_arg command;
  StringToOpaque(request, &command.arg.request);
  for (int i = 0; i < replica_state_->getMaxNumClient(); ++i) {
    if (i != replica_state_->getSelfRank()) {
      auto* replica_client = replica_client_set_->getReplicaClient(i);
      if (replica_client) {
        // Result is ignored.
        command.vs.ts = 1;
        replica_client->replicate(command);
        replica_client_set_->releaseReplicaClient(i);
      }
    }
  }
  return back_store_->Run(request);
}
