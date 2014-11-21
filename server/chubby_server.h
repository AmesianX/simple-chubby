#ifndef _SERVER_CHUBBY_SERVER_INCLUDED_
#define _SERVER_CHUBBY_SERVER_INCLUDED_ 1

#include <map>
#include <xdrpp/server.h>

namespace xdr {

typedef uint32_t SessionId;

class chubby_service_base_interface {
 public:
  const uint32_t prog_;
  const uint32_t vers_;
  chubby_service_base_interface(uint32_t prog, uint32_t vers)
      : prog_(prog), vers_(vers) {}
  ~chubby_service_base_interface() {}
  virtual void asynchronized_process(
      const rpc_msg &chdr, xdr_get &g, SessionId session_id, msg_sock* ms) = 0;
};

template<typename T> class chubby_service_base : public chubby_service_base_interface {
  using interface = typename T::rpc_interface_type;
  T &server_;

 public:

  chubby_service_base(T &server)
    : chubby_service_base_interface(interface::program, interface::version), server_(server) {}
  ~chubby_service_base() {}

  void asynchronized_process(const rpc_msg &chdr, xdr_get &g,
                             SessionId session_id, msg_sock* ms) override {
    // Check header.
    if (chdr.body.mtype() != CALL || chdr.body.cbody().rpcvers != 2
        || chdr.body.cbody().prog != prog_ || chdr.body.cbody().vers != vers_)
      return;

    // Build response header.
    uint32_t xid = chdr.xid;

    if (!interface::call_dispatch(*this, chdr.body.cbody().proc,
                                  session_id, xid, g)) {
      rpc_msg rhdr;
      rhdr.xid = chdr.xid;
      rhdr.body.mtype(REPLY).rbody().stat(MSG_ACCEPTED)
          .areply().reply_data.stat(SUCCESS);
      rhdr.body.rbody().areply().reply_data.stat(PROC_UNAVAIL);
      ms->putmsg(xdr_to_msg(rhdr));
    }
  }

  template<typename P> typename std::enable_if<
    !std::is_same<void, typename P::res_type>::value>::type
  dispatch(SessionId session_id, uint32_t xid, xdr_get &g) {
    std::unique_ptr<typename P::arg_wire_type>
      arg(new typename P::arg_wire_type);
    archive(g, *arg);
    if (g.p_ != g.e_)
      throw xdr_bad_message_size("synchronous_server did not consume"
				 " whole message");

    std::unique_ptr<typename P::res_type> res =
      P::dispatch_dropvoid(server_, std::move(arg), session_id, xid);
  }
  template<typename P> typename std::enable_if<
    std::is_same<void, typename P::res_type>::value>::type
  dispatch(SessionId session_id, uint32_t xid, xdr_get &g) {
    std::unique_ptr<typename P::arg_wire_type>
      arg(new typename P::arg_wire_type);
    archive(g, *arg);
    if (g.p_ != g.e_)
      throw xdr_bad_message_size("synchronous_server did not consume"
				 " whole message");
    P::dispatch_dropvoid(server_, std::move(arg), session_id, xid);
  }

};

class chubby_server {
  std::map<uint32_t,
	   std::map<uint32_t, std::unique_ptr<chubby_service_base_interface>>> servers_;
  pollset ps_;
  unique_fd listen_fd_;

  void accept_cb();
  void receive_cb(SessionId session_id, msg_ptr mp);
  void asynchronized_dispatch(SessionId session_id, msg_ptr mp);
  void register_base_service(chubby_service_base_interface *s) {
    servers_[s->prog_][s->vers_].reset(s);
  }

 public:
  chubby_server(unique_fd &&fd);
  chubby_server() : chubby_server(unique_fd(-1)) {}
  virtual ~chubby_server();
  template<typename T> void register_service(T &t) {
    register_base_service(new chubby_service_base<T>(t));
  }

  void run();
  template<typename T> bool reply(SessionId session_id,
                                  uint32_t xid, std::unique_ptr<T> result) {
    rpc_msg rhdr;
    rhdr.xid = xid;
    rhdr.body.mtype(REPLY).rbody().stat(MSG_ACCEPTED)
      .areply().reply_data.stat(SUCCESS);
    sessionid_to_msgsock_map_[session_id]->putmsg(xdr_to_msg(rhdr, *result));
  }
 private:
  std::map<int, SessionId> fd_to_sessionid_map_;
  std::map<SessionId, int> sessionid_to_fd_map_;
  std::map<SessionId, msg_sock*> sessionid_to_msgsock_map_;
  SessionId session_id_allocator_ {0};
  SessionId registerSession(int fd, msg_sock* ms) {
    SessionId session_id = ++session_id_allocator_;
    fd_to_sessionid_map_[fd] =session_id;
    sessionid_to_fd_map_[session_id] = fd;
    sessionid_to_msgsock_map_[session_id] = ms;
    return session_id;
  }
  void deregisterSession(SessionId session_id) {
    int fd = sessionid_to_fd_map_[session_id];
    msg_sock* ms = sessionid_to_msgsock_map_[session_id];
    delete ms;
    fd_to_sessionid_map_.erase(fd);
    sessionid_to_fd_map_.erase(session_id);
    sessionid_to_msgsock_map_.erase(session_id);
    std::cout << "Turned donw Session #" << session_id << std::endl;
  }

};


}  // namespace xdr

#endif  // _SERVER_CHUBBY_SERVER_INCLUDED_
