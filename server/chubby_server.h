#ifndef _SERVER_CHUBBY_SERVER_INCLUDED_
#define _SERVER_CHUBBY_SERVER_INCLUDED_ 1

#include <map>
#include <xdrpp/server.h>

namespace xdr {

class chubby_server {
  std::map<uint32_t,
	   std::map<uint32_t, std::unique_ptr<service_base>>> servers_;
  pollset ps_;
  unique_fd listen_fd_;

  void accept_cb();
  void receive_cb(msg_sock *ms, msg_ptr mp);
  void asynchronized_dispatch(msg_ptr mp, msg_sock *ms);

public:
  chubby_server(unique_fd &&fd);
  chubby_server() : chubby_server(unique_fd(-1)) {}
  virtual ~chubby_server();
  void register_service(service_base *s) {
    servers_[s->prog_][s->vers_].reset(s);
  }

  void run();
};


}  // namespace xdr

#endif  // _SERVER_CHUBBY_SERVER_INCLUDED_
