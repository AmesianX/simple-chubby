#include "server/chubby_client.h"
#include <xdrpp/socket.h>

namespace xdr {

chubby_client::chubby_client(int fd) : fd_(fd)
{
  set_close_on_exec(fd_);
  msg_sock *ms = new msg_sock(ps_, fd);
  ms->setrcb(std::bind(&chubby_client::poll_recv_cb, this, ms,
                       std::placeholders::_1));
  pollth_ = std::thread(std::bind(&chubby_client::poll_loop, this));
  evcbth_ = std::thread(std::bind(&chubby_client::evcb_loop, this));
}

chubby_client::chubby_client(chubby_client &&c) : fd_(c.fd_)
{
  pollth_.swap(c.pollth_);
  evcbth_.swap(c.evcbth_);
  event_queue_.swap(c.event_queue_);
  reply_queue_.swap(c.reply_queue_);
}

chubby_client::~chubby_client() {
  ps_.fd_cb(fd_, pollset::Read);
}

void chubby_client::poll_loop() {
  if (xdr_trace_client)
    std::clog << "Chubby client backgroud thread is up ..." << std::endl;
  while (ps_.pending())
    ps_.poll();
  if (xdr_trace_client)
    std::clog << "Chubby client backgroud thread terminated ..." << std::endl;
}

void chubby_client::evcb_loop() {
  while (true) {
    msg_ptr m;
    {
      std::unique_lock<std::mutex> lock(elk_);
      ecv_.wait(lock, [this]{ return !event_queue_.empty(); });
      m = std::move(event_queue_.front());
      event_queue_.pop_front();
    }

    xdr_get g(m);
    rpc_msg hdr;
    archive(g, hdr);

    try {
      // check rpc version.
      if (hdr.body.cbody().rpcvers != 2)
        throw xdr_runtime_error("chubby_client: "
                                "rpc version doesn't match for event message");
      // check program
      if (hdr.body.cbody().prog != evP::interface_type::program)
        throw xdr_runtime_error("chubby_client: "
                                "rpc event program doesn't match for event message");
      // check version
      if (hdr.body.cbody().vers != evP::interface_type::version)
        throw xdr_runtime_error("chubby_client: "
                                "rpc event version doesn't match for event message");

      evP::arg_wire_type arg;
      archive(g, arg);
      if (g.p_ != g.e_)
        throw xdr_bad_message_size("chubby_client: "
                                   "did not consume whole message");

      auto func = ecbs_.find(static_cast<int>(arg.event));
      if (func != ecbs_.end())
        func->second(arg.fname);
      else {
        std::string s = "No callback registered for event ";
        s += std::to_string(arg.event);
        std::clog << s << std::endl;
      }
    }
    catch (const xdr_runtime_error &e) {
      std::cerr << e.what() << std::endl;
    }

  }
}

void chubby_client::poll_recv_cb(msg_sock *ms, msg_ptr mp) {
  if (!mp) {
    delete ms;
    return;
  }

  // Get header
  rpc_msg hdr;
  std::unique_ptr<xdr_get> g(new xdr_get(mp));
  archive(*g, hdr);

  // For replies, push to the \c reply_queue_
  if (hdr.body.mtype() == REPLY) {
    {
      // mutex region begin
      std::lock_guard<std::mutex> lock(rlk_);
      reply_queue_.push_back(std::move(mp));
      // mutex region end
    }
    rcv_.notify_one();
  }

  // For calls (events), call callbacks directly
  if (hdr.body.mtype() == CALL) {
    {
      // mutex region begin
      std::lock_guard<std::mutex> lock(elk_);
      event_queue_.push_back(std::move(mp));
      // mutex region end
    }
    ecv_.notify_one();
  }
}

}  // namespace xdr
