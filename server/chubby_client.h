#ifndef _SERVER_CHUBBY_CLIENT_INCLUDED_
#define _SERVER_CHUBBY_CLIENT_INCLUDED_ 1

#include <thread>
#include <list>
#include <mutex>
#include <condition_variable>
#include <tuple>
#include <unistd.h>
#include <xdrpp/exception.h>
#include <xdrpp/rpc_msg.hh>
#include <xdrpp/pollset.h>
#include <xdrpp/srpc.h>

#include "include/event.hh"
#include "include/server.hh"

namespace xdr {

extern bool xdr_trace_client;

struct server_addr_t {
  std::string host;
  std::string service;

  server_addr_t(const std::string h, const std::string s) : host(h), service(s) {}
};

class chubby_client {

  int fd_;
  uint64_t id_;

  std::vector<server_addr_t> server_addr_;

  std::thread pollth_;
  pollset ps_;

  std::thread evcbth_;
  std::list<msg_ptr> event_queue_;
  std::mutex elk_;
  std::condition_variable ecv_;

  std::list<msg_ptr> reply_queue_;
  std::mutex rlk_;
  std::condition_variable rcv_;

  std::mutex rpclk_;

  // client event callback
  // one callback for each event
  typedef std::function<void(std::string)> EventCallback;
  std::unordered_map<int, EventCallback> ecbs_;

  // local info to help server recover
  std::mutex bkup_lk_;
  const FileHandler* lk_acq_fd_bkup_;
  rpc_msg hdr_bkup_;
  std::list<std::tuple<FileHandler, Mode>> event_bkup_;

private:
  using evP = handler_v1::event_callback_t;

  void connect_server_and_recover(bool is_init = false);

  static void moveret(std::unique_ptr<xdr_void> &) {}
  template<typename T> static T &&moveret(T &t) { return std::move(t); }

  void poll_loop();
  void evcb_loop();
  void poll_recv_cb(msg_sock *ms, msg_ptr mp);

  template<typename P>
  void store_bkup_before_call(const rpc_msg hdr,
                              const typename P::arg_wire_type &a) {}
  template<typename P>
  void store_bkup_after_call(const typename P::arg_wire_type &a,
                             const std::unique_ptr<typename P::res_wire_type>& r) {}

public:
  chubby_client(const std::string& server_addr_file);
  ~chubby_client();
  chubby_client(chubby_client &&c);
  chubby_client(const chubby_client &c) = delete;

  void register_callback(ChubbyEvent e, EventCallback cb) {
    ecbs_[static_cast<int>(e)] = cb;
  }
  void delete_callback(ChubbyEvent e) {
    ecbs_.erase(static_cast<int>(e));
  }

  template<typename P> typename P::res_type invoke() {
    return this->template invoke<P>(xdr::xdr_void{});
  }

  template<typename P> typename std::conditional<
    std::is_void<typename P::res_type>::value, void,
    std::unique_ptr<typename P::res_type>>::type
  invoke(const typename P::arg_wire_type &a) {

    std::lock_guard<std::mutex> l(rpclk_);

    rpc_msg hdr;
    prepare_call<P>(hdr);
    uint32_t xid = hdr.xid;

    // send request
    if (xdr_trace_client) {
      std::string s = "CALL ";
      s += P::proc_name;
      s += " -> [xid " + std::to_string(xid) + "]";
      std::clog << xdr_to_string(a, s.c_str());
    }

    store_bkup_before_call<P>(hdr, a);

    // FIXME: not guarantee to succeed if connection fails during write
    write_message(fd_, xdr_to_msg(hdr, a));

    // wait for reply
    msg_ptr m;
    {
      std::unique_lock<std::mutex> lock(rlk_);
      rcv_.wait(lock, [this]{ return !reply_queue_.empty(); });
      m = std::move(reply_queue_.front());
      reply_queue_.pop_front();
    }

    xdr_get g(m);
    archive(g, hdr);
    check_call_hdr(hdr);
    if (hdr.xid != xid)
      throw xdr_runtime_error("chubby_client: unexpected xid");

    std::unique_ptr<typename P::res_wire_type> r{new typename P::res_wire_type};
    archive(g, *r);
    if (g.p_ != g.e_)
      throw xdr_bad_message_size("chubby_client: "
                                 "did not consume whole message");
    if (xdr_trace_client) {
      std::string s = "REPLY ";
      s += P::proc_name;
      s += " <- [xid " + std::to_string(xid) + "]";
      std::clog << xdr_to_string(*r, s.c_str());
    }

    store_bkup_after_call<P>(a, r);

    return moveret(r);
  }
};

template<typename T> using chubby_client_handler =
  typename T::template client<chubby_client>;

}  // namespace xdr

#endif  // _SERVER_CHUBBY_CLIENT_INCLUDED_
