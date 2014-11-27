
#include <iostream>
#include <server/chubby_server.h>

namespace xdr {

namespace {

rpc_msg &
rpc_mkerr(rpc_msg &m, accept_stat stat)
{
  m.body.mtype(REPLY).rbody().stat(MSG_ACCEPTED).areply()
    .reply_data.stat(stat);
  return m;
}

rpc_msg &
rpc_mkerr(rpc_msg &m, reject_stat stat)
{
  m.body.mtype(REPLY).rbody().stat(MSG_DENIED).rreply().stat(stat);
  switch(stat) {
  case RPC_MISMATCH:
    m.body.rbody().rreply().mismatch_info().low = 2;
    m.body.rbody().rreply().mismatch_info().high = 2;
    break;
  case AUTH_ERROR:
    m.body.rbody().rreply().rj_why() = AUTH_FAILED;
    break;
  }
  return m;
}

}  // namespace

// Setup the polling on the socket "fd". Callback is accept_cb.
chubby_server::chubby_server(unique_fd &&fd)
    : listen_fd_(fd ? std::move(fd) : tcp_listen()) {
  set_close_on_exec(listen_fd_.get());
  ps_.fd_cb(listen_fd_.get(), pollset::Read,
	    std::bind(&chubby_server::accept_cb, this));
}

chubby_server::~chubby_server() {
  ps_.fd_cb(listen_fd_.get(), pollset::Read);
}

void chubby_server::accept_cb() {
  int fd = accept(listen_fd_.get(), nullptr, 0);
  if (fd == -1) {
    std::cerr << "chubby_server: accept: " << std::strerror(errno)
	      << std::endl;
    return;
  }
  set_close_on_exec(fd);
  msg_sock *ms = new msg_sock(ps_, fd);
  sockaddr saddr;
  socklen_t slen;
  getpeername(fd, &saddr, &slen);
  std::string host, serv;
  get_numinfo(&saddr, slen, &host, &serv);
  SessionId session_id = registerSession(fd, ms, host + serv);
  std::cout << " New session #" << session_id << " " << host << " "
      << serv << std::endl;
  ms->setrcb(std::bind(&chubby_server::receive_cb, this, session_id,
		       std::placeholders::_1));
}

void chubby_server::receive_cb(SessionId session_id, msg_ptr mp) {
  if (!mp) {
    deregisterSession(session_id);
    return;
  }
  try {
    asynchronized_dispatch(session_id, std::move(mp));
  } catch (const xdr_runtime_error &e) {
    std::cerr << e.what() << std::endl;
    deregisterSession(session_id);
  }
}

void chubby_server::asynchronized_dispatch(SessionId session_id, msg_ptr mp) {
  auto iter_ms = sessionid_to_msgsock_map_.find(session_id);
  if (iter_ms == sessionid_to_msgsock_map_.end()) {
    std::cerr << "Session id is invalid." << std::endl;
    return;
  }
  msg_sock* ms = iter_ms->second;
  // ms->putmsg
  // Unmarshell the message.
  xdr_get g(mp);
  rpc_msg hdr;
  archive(g, hdr);
  // check type.
  if (hdr.body.mtype() != CALL)
    throw xdr_runtime_error("rpc_server received non-CALL message");
  // Check rpc version.
  if (hdr.body.cbody().rpcvers != 2) {
    ms->putmsg(xdr_to_msg(rpc_mkerr(hdr, RPC_MISMATCH)));
    return;
  }
  // Identify program.
  auto prog = servers_.find(hdr.body.cbody().prog);
  if (prog == servers_.end()) {
    ms->putmsg(xdr_to_msg(rpc_mkerr(hdr, PROG_UNAVAIL)));
    return;
  }
  // Identify version.
  auto vers = prog->second.find(hdr.body.cbody().vers);
  if (vers == prog->second.end()) {
    rpc_mkerr(hdr, PROG_MISMATCH);
    hdr.body.rbody().areply().reply_data.mismatch_info().low =
      prog->second.cbegin()->first;
    hdr.body.rbody().areply().reply_data.mismatch_info().high =
      prog->second.crbegin()->first;
    ms->putmsg(xdr_to_msg(hdr));
    return;
  }

  try {
    // Run it async.
    vers->second->asynchronized_process(hdr, g, session_id, ms);
    return;
  } catch (const xdr_runtime_error &e) {
    std::cerr << xdr_to_string(hdr, e.what());
    ms->putmsg(xdr_to_msg(rpc_mkerr(hdr, GARBAGE_ARGS)));
    return;
  }
}

void chubby_server::run() {
  while (ps_.pending())
    ps_.poll();
}

}  // namespace xdr
