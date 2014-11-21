#ifndef _SERVER_CHUBBY_CLIENT_INCLUDED_
#define _SERVER_CHUBBY_CLIENT_INCLUDED_ 1

#include <list>
#include <pthread.h>
#include <unistd.h>
#include <tuple>
#include <xdrpp/exception.h>
#include <xdrpp/rpc_msg.hh>
#include <xdrpp/srpc.h>

namespace xdr {

class chubby_client {
  pthread_mutex_t lock_;
  pthread_cond_t cond_;
  bool waiting_for_reply_;
  bool fetching_needed_;
  typedef std::tuple<rpc_msg, std::unique_ptr<xdr_get>, msg_ptr> QueueElement;
  std::list<QueueElement> call_queue_;

  const int fd_;

  static void moveret(std::unique_ptr<xdr_void> &) {}
  template<typename T> static T &&moveret(T &t) { return std::move(t); }

public:
  chubby_client(int fd);
  chubby_client(const chubby_client &c);

  template<typename P> typename P::res_type invoke() {
    return this->template invoke<P>(xdr::xdr_void{});
  }

  void getEvent();

  void FetchMessageIfNeeded();

  template<typename P> typename std::conditional<
    std::is_void<typename P::res_type>::value, void,
    std::unique_ptr<typename P::res_type>>::type
  invoke(const typename P::arg_wire_type &a) {
    rpc_msg hdr;
    prepare_call<P>(hdr);
    uint32_t xid = hdr.xid;

    write_message(fd_, xdr_to_msg(hdr, a));

    FetchMessageIfNeeded();

    pthread_mutex_lock(&lock_);
    while (call_queue_.empty() ||
           std::get<0>(call_queue_.front()).body.mtype() != REPLY) {
      if (call_queue_.empty()) {
        pthread_mutex_unlock(&lock_);
        FetchMessageIfNeeded();
        pthread_mutex_lock(&lock_);
        continue;
      }
      pthread_cond_wait(&cond_, &lock_);
    }
    rpc_msg recv_hdr;
    recv_hdr = std::get<0>(call_queue_.front());
    check_call_hdr(recv_hdr);
    std::unique_ptr<xdr_get> g;
    msg_ptr m;
    g = std::move(std::get<1>(call_queue_.front()));
    m = std::move(std::get<2>(call_queue_.front()));
    call_queue_.pop_front();
    pthread_cond_broadcast(&cond_);
    pthread_mutex_unlock(&lock_);

    check_call_hdr(recv_hdr);
    if (recv_hdr.xid != xid)
      throw xdr_runtime_error("chubby_client: unexpected xid");

    std::unique_ptr<typename P::res_wire_type> r{new typename P::res_wire_type};
    archive(*g, *r);
    if (g->p_ != g->e_)
      throw xdr_bad_message_size("chubby_client: "
                                 "did not consume whole message");
    return moveret(r);
  }
};

template<typename T> using chubby_client_handler =
  typename T::template client<chubby_client>;

}  // namespace xdr
#endif  // _SERVER_CHUBBY_CLIENT_INCLUDED_
