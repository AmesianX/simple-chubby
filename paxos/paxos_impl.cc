// Scaffolding originally generated from paxos/paxos.x.
// Edit to add functionality.

#include "paxos/replica_client_set.hh"
#include "paxos/replica_state.hh"
#include "paxos/helper.hh"
#include "paxos/execute_replicate_engine.hh"

#include "paxos/paxos_impl.hh"

std::unique_ptr<replicate_res>
paxos_v1_server::replicate(std::unique_ptr<replicate_arg> arg)
{
  std::unique_ptr<replicate_res> res(new replicate_res);
  if (arg->vs.ts == 0) {
    // new view.
    init_view_request request;
    xdr::msg_ptr message(xdr::message_t::alloc(arg->arg.request.size() - 4));
    memcpy(message->raw_data(), arg->arg.request.data(), arg->arg.request.size());
    xdr_from_msg(message, request);
    replica_state_->BeginAccess();
    printf("[PAXOS] New leader promoted: rank#%d\n",
           replica_state_->getClientUseAddressRank(request.newview.primary.addr));
    replica_state_->isLeader = false;
    replica_state_->view = request.newview;
    replica_state_->mode = ReplicaState::VC_ACTIVE;
    replica_state_->EndAccess();

    return res;
  } else {
    execute_replicate_engine_->BackUp(OpaqueToString(arg->arg.request));
  }
}

std::unique_ptr<view_change_res>
paxos_v1_server::view_change(std::unique_ptr<view_change_arg> arg)
{
  std::unique_ptr<view_change_res> res(new view_change_res);

  // TODO: viewid_t compare: cid_t less is larger
  replica_state_->BeginAccess();
  // TODO: change state to underling?

  viewid_t old_vid = arg->oldview.vid;
  viewid_t new_vid = arg->newvid;
  viewid_t cur_vid = replica_state_->view.vid;

  if (old_vid < cur_vid) {
    // pp8 case 1: reject
    res->accepted(false);
    res->reject().oldview = replica_state_->view;
    res->reject().newvid = replica_state_->proposed_vid;
  } else if (new_vid < replica_state_->proposed_vid) {
    // pp8 case 2: reject
    if (cur_vid < old_vid) {
      replica_state_->view = arg->oldview;
    }
    res->accepted(false);
    res->reject().oldview = replica_state_->view;
    res->reject().newvid = replica_state_->proposed_vid;
  } else if (old_vid == cur_vid && !isNullView(replica_state_->accepted_view)) {
    // pp8 case 3: accept
    replica_state_->mode == ReplicaState::VC_UNDERLING;
    replica_state_->proposed_vid = arg->newvid;
    res->accepted(true);
    res->accept().myid = replica_state_->getReplicaAddress(replica_state_->getSelfRank());
    res->accept().newview = replica_state_->accepted_view;
  } else {
    // pp8 case 4: accept
    replica_state_->mode == ReplicaState::VC_UNDERLING;
    replica_state_->view = arg->oldview;
    replica_state_->proposed_vid = arg->newvid;
    setNullView(replica_state_->accepted_view);
    res->accepted(true);
    res->accept().myid = replica_state_->getReplicaAddress(replica_state_->getSelfRank());
    setNullView(res->accept().newview);
  }
  replica_state_->EndAccess();

  return res;
}

std::unique_ptr<new_view_res>
paxos_v1_server::new_view(std::unique_ptr<new_view_arg> arg)
{
  std::unique_ptr<new_view_res> res(new new_view_res);

  replica_state_->BeginAccess();
  if (arg->view.vid == replica_state_->proposed_vid) {
    replica_state_->accepted_view = arg->view;
    res->accepted = true;
  } else {
    res->accepted = false;
  }
  // FIXME: arg->latest is ignored
  replica_state_->EndAccess();

  return res;
}

std::unique_ptr<init_view_res>
paxos_v1_server::init_view(std::unique_ptr<init_view_arg> arg)
{
  std::unique_ptr<init_view_res> res(new init_view_res);

  replica_state_->BeginAccess();
  replica_state_->markToPromote(arg->view);
  replica_state_->EndAccess();

  res->succeed = true;
  return res;
}

std::unique_ptr<execute_res>
paxos_client_v1_server::execute(std::unique_ptr<execute_arg> arg)
{
  std::unique_ptr<execute_res> res(new execute_res);
  replica_state_->BeginAccess();
  bool is_leader = replica_state_->isLeader;
  replica_state_->EndAccess();
  if (!is_leader) {
    res->ok(false);
    return res;
  }
  std::string result = execute_replicate_engine_->replicateCommand(
      OpaqueToString(arg->request));
  res->ok(true);
  StringToOpaque(result, &res->reply());
  return res;
}
