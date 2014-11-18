#ifndef _SERVER_CHUBBY_SERVER_INCLUDED_
#define _SERVER_CHUBBY_SERVER_INCLUDED_ 1

#include <map>
#include <xdrpp/server.h>

namespace xdr {

class chubby_service_base_interface {
 public:
  const uint32_t prog_;
  const uint32_t vers_;
  chubby_service_base_interface(uint32_t prog, uint32_t vers)
      : prog_(prog), vers_(vers) {}
  ~chubby_service_base_interface() {}
  virtual void asynchronized_process(
      const rpc_msg &chdr, xdr_get &g, msg_sock *ms) = 0;
};

template<typename T> class chubby_service_base : public chubby_service_base_interface {
  using interface = typename T::rpc_interface_type;
  T &server_;

 public:

  chubby_service_base(T &server)
    : chubby_service_base_interface(interface::program, interface::version), server_(server) {}
  ~chubby_service_base() {}

  void asynchronized_process(const rpc_msg &chdr, xdr_get &g, msg_sock *ms)
      override {
    // Check header.
    if (chdr.body.mtype() != CALL || chdr.body.cbody().rpcvers != 2
        || chdr.body.cbody().prog != prog_ || chdr.body.cbody().vers != vers_)
      return;

    // Build response header.
    rpc_msg rhdr;
    rhdr.xid = chdr.xid;
    rhdr.body.mtype(REPLY).rbody().stat(MSG_ACCEPTED)
      .areply().reply_data.stat(SUCCESS);

    msg_ptr ret;
    interface::call_dispatch(*this, chdr.body.cbody().proc, ms, g, rhdr, ret);
    /*
    if (!interface::call_dispatch(*this, chdr.body.cbody().proc,
				  g, rhdr, ret)) {
      // Cannot find corresponding procedure.
      return;
      // rhdr.body.rbody().areply().reply_data.stat(PROC_UNAVAIL);
      // ret = xdr_to_msg(rhdr);
    }
    return ret;
    */
  }

  template<typename P> typename std::enable_if<
    !std::is_same<void, typename P::res_type>::value>::type
  dispatch(msg_sock *ms, xdr_get &g, rpc_msg rhdr, msg_ptr &ret) {
    std::unique_ptr<typename P::arg_wire_type>
      arg(new typename P::arg_wire_type);
    archive(g, *arg);
    if (g.p_ != g.e_)
      throw xdr_bad_message_size("synchronous_server did not consume"
				 " whole message");

    std::unique_ptr<typename P::res_type> res =
      P::dispatch_dropvoid(server_, std::move(arg), rhdr, ms);
    //ms->put_msg(xdr_to_msg(rhdr, *res));
    //ret = xdr_to_msg(rhdr, *res);
  }
  template<typename P> typename std::enable_if<
    std::is_same<void, typename P::res_type>::value>::type
  dispatch(msg_sock *ms, xdr_get &g, rpc_msg rhdr, msg_ptr &ret) {
    std::unique_ptr<typename P::arg_wire_type>
      arg(new typename P::arg_wire_type);
    archive(g, *arg);
    if (g.p_ != g.e_)
      throw xdr_bad_message_size("synchronous_server did not consume"
				 " whole message");
    P::dispatch_dropvoid(server_, std::move(arg), rhdr, ms);
    // Not handled.
    // ret = xdr_to_msg(rhdr);
  }

};

class chubby_server {
  std::map<uint32_t,
	   std::map<uint32_t, std::unique_ptr<chubby_service_base_interface>>> servers_;
  pollset ps_;
  unique_fd listen_fd_;

  void accept_cb();
  void receive_cb(msg_sock *ms, msg_ptr mp);
  void asynchronized_dispatch(msg_ptr mp, msg_sock *ms);
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
};


}  // namespace xdr

#endif  // _SERVER_CHUBBY_SERVER_INCLUDED_
