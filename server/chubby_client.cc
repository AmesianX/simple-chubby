#include "server/chubby_client.h"

namespace xdr {

chubby_client::chubby_client(int fd) : fd_(fd) {
  pthread_mutex_init(&lock_, NULL);
  pthread_cond_init(&cond_, NULL);
  waiting_for_reply_ = false;
  fetching_needed_ = true;
}
chubby_client::chubby_client(const chubby_client &c) : fd_(c.fd_) {
  pthread_mutex_init(&lock_, NULL);
  pthread_cond_init(&cond_, NULL);
  waiting_for_reply_ = false;
  fetching_needed_ = true;
}

void chubby_client::getEvent() {
  FetchMessageIfNeeded();
  pthread_mutex_lock(&lock_);
  while (call_queue_.empty() ||
         std::get<0>(call_queue_.front()).body.mtype() != CALL) {
    if (call_queue_.empty()) {
      pthread_mutex_unlock(&lock_);
      FetchMessageIfNeeded();
      pthread_mutex_lock(&lock_);
      continue;
    }
    pthread_cond_wait(&cond_, &lock_);
  }
  auto& front = call_queue_.front();
  rpc_msg hdr;
  std::unique_ptr<xdr_get> g;
  msg_ptr m;
  hdr = std::get<0>(front);
  g = std::move(std::get<1>(front));
  m = std::move(std::get<2>(front));
  call_queue_.pop_front();
  pthread_cond_broadcast(&cond_);
  pthread_mutex_unlock(&lock_);
  xdr::xstring<> temp;
  archive(*g, temp);
  std::cout << "Event received: " << temp << std::endl;
}

void chubby_client::FetchMessageIfNeeded() {
  pthread_mutex_lock(&lock_);
  if (!fetching_needed_) {
    pthread_mutex_unlock(&lock_);
    return;
  } else {
    fetching_needed_ = false;
    pthread_mutex_unlock(&lock_);
  }
  msg_ptr m = read_message(fd_);
  std::unique_ptr<xdr_get> g(new xdr_get(m));
  rpc_msg hdr;
  archive(*g, hdr);

  pthread_mutex_lock(&lock_);
  call_queue_.push_back(make_tuple(hdr, std::move(g), std::move(m)));
  fetching_needed_ = true;
  pthread_cond_broadcast(&cond_);
  pthread_mutex_unlock(&lock_);
}

}  // namespace xdr
