#include "server/chubby_client.h"
#include "xdrpp/socket.h"

namespace xdr {

chubby_client::chubby_client(int fd) : fd_(fd)
{
  set_close_on_exec(fd_);
  msg_sock *ms = new msg_sock(ps_, fd);
  ms->setrcb(std::bind(&chubby_client::bg_recv_cb, this, ms,
                       std::placeholders::_1));
  bgth_ = std::thread(std::bind(&chubby_client::bg_loop, this));
}

chubby_client::chubby_client(chubby_client &&c) : fd_(c.fd_)
{
  bgth_.swap(c.bgth_);
  reply_queue_.swap(c.reply_queue_);
}

chubby_client::~chubby_client() {
  ps_.fd_cb(fd_, pollset::Read);
}

void chubby_client::bg_loop() {
  if (xdr_trace_client)
    std::clog << "Chubby client backgroud thread is up ..." << std::endl;
  while (ps_.pending())
    ps_.poll();
  if (xdr_trace_client)
    std::clog << "Chubby client backgroud thread terminated ..." << std::endl;
}

void chubby_client::bg_recv_cb(msg_sock *ms, msg_ptr mp) {
  if (!mp) {
    delete ms;
    return;
  }
  try {
    rpc_msg hdr;
    std::unique_ptr<xdr_get> g(new xdr_get(mp));
    archive(*g, hdr);

    // for replies, push to the \c reply_queue_
    if (hdr.body.mtype() == REPLY) {
      {
        // mutex region begin
        std::lock_guard<std::mutex> lock(lk_);
        reply_queue_.push_back(std::move(mp));
        // mutex region end
      }
      cv_.notify_one();
    }

    // for calls (events), call callbacks directly
    if (hdr.body.mtype() == CALL) {
      // TODO
      std::clog << "An event received, do nothing for now";
    }
  }
  catch (const xdr_runtime_error &e) {
    std::cerr << e.what() << std::endl;
    delete ms;
  }
}

}  // namespace xdr
