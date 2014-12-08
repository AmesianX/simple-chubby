#include "server/chubby_client.h"
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <sys/time.h>
#include <xdrpp/socket.h>
#include <exception>

namespace xdr {

chubby_client::chubby_client(const std::string& server_addr_file) {
  // use timestamp as the id
  struct timeval tv;
  gettimeofday(&tv, nullptr);
  id_ = tv.tv_sec * 1000000 + tv.tv_usec;

  // read in server config
  std::ifstream ifs;
  ifs.open(server_addr_file);
  std::string line;
  // skip first two lines
  std::getline(ifs, line);
  std::getline(ifs, line);
  while(std::getline(ifs, line)) {
    // skip server port for paxos
    std::getline(ifs, line);
    // use server port for client
    if (!line.empty()) {
      std::string::size_type colon_pos = line.find(":");
      if (colon_pos == std::string::npos) continue;
      server_addr_.emplace_back(line.substr(0, colon_pos), line.substr(colon_pos + 1));
    }
  }
  ifs.close();

  // try connect server
  lk_acq_fd_bkup_ = nullptr;
  event_bkup_.clear();
  fd_ = -1;
  connect_server_and_recover(true);

  if (xdr_trace_client) {
    std::clog << "Chubby client connected to server ..." << std::endl;
  }

  pollth_ = std::thread(std::bind(&chubby_client::poll_loop, this));
  evcbth_ = std::thread(std::bind(&chubby_client::evcb_loop, this));
  evcbth_.detach();
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
  pollth_.join();
}

void chubby_client::connect_server_and_recover(bool is_init) {

  while (fd_ == -1) {
    for (const auto& addr : server_addr_) {
      try {
        auto fd = tcp_connect(addr.host.c_str(), addr.service.c_str());
        fd_ = fd.release();

        set_close_on_exec(fd_);

        // start session
        auto clnt = new synchronous_client_base(fd_);
        auto r = clnt->template invoke<api_v1::startSession_t>(std::to_string(id_));
        delete clnt;

        if (r->discriminant() == 0) {
          if (r->val()) {
            break;
          }
        }

        close(fd_);
        fd_ = -1;

      } catch (std::exception& e) {
        fd_ = -1;
        sleep(1);
      }
    }
  }

  {
    std::lock_guard<std::mutex> lock(bkup_lk_);
    for (auto& it : event_bkup_) {
      ArgReopen arg;
      arg.fd = std::get<0>(it);
      arg.mode = std::get<1>(it);
      auto clnt = new synchronous_client_base(fd_);
      auto r = clnt->template invoke<api_v1::fileReopen_t>(arg);
      delete clnt;
      // ignore failure for reopen
    }
    {
      std::lock_guard<std::mutex> lock(rlk_);
      if (lk_acq_fd_bkup_ != nullptr) {
        write_message(fd_, xdr_to_msg(hdr_bkup_, *lk_acq_fd_bkup_));
      }
    }
  }

  msg_sock *ms = new msg_sock(ps_, fd_);
  ms->setrcb(std::bind(&chubby_client::poll_recv_cb, this, ms,
                       std::placeholders::_1));
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
        //std::string s = "No callback registered for event ";
        //s += std::to_string(arg.event);
        //std::clog << s << std::endl;
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

    close(fd_);
    fd_ = -1;
    connect_server_and_recover();
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

template<>
void chubby_client::store_bkup_before_call<api_v1::acquire_t>(const rpc_msg hdr,
                                                              const FileHandler &a) {
  std::lock_guard<std::mutex> lock(bkup_lk_);
  lk_acq_fd_bkup_ = &a;
  hdr_bkup_ = hdr;
}
template<>
void chubby_client::store_bkup_after_call<api_v1::acquire_t>(const FileHandler &a,
                                                             const std::unique_ptr<RetBool>& r) {
  std::lock_guard<std::mutex> lock(bkup_lk_);
  lk_acq_fd_bkup_ = nullptr;
}

template<>
void chubby_client::store_bkup_after_call<api_v1::fileClose_t>(const FileHandler &a,
                                                               const std::unique_ptr<RetBool>& r) {
  std::lock_guard<std::mutex> lock(bkup_lk_);
  auto it = event_bkup_.begin();
  while (it != event_bkup_.end()) {
    if (std::get<0>(*it).file_name == a.file_name) {
      it = event_bkup_.erase(it);
    }
  }
}

template<>
void chubby_client::store_bkup_after_call<api_v1::fileOpen_t>(const ArgOpen &a,
                                                              const std::unique_ptr<RetFd>& r) {
  if (r->discriminant() == 0) {
    std::lock_guard<std::mutex> lock(bkup_lk_);
    event_bkup_.push_back(std::make_pair(r->val(), a.mode));
  }
}


}  // namespace xdr
