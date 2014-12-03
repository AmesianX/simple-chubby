#ifndef __PAXOS_BACK_STORE_HH_INCLUDED__
#define __PAXOS_BACK_STORE_HH_INCLUDED__

#include <iostream>
#include <string>

class BackStoreInterface {
 public:
  virtual void Initialize() = 0;
  virtual std::string Run(const std::string input) = 0;
};
class BackStore : public BackStoreInterface {
 public:
  virtual void Initialize() {
    std::cout << "    USER: back store initialized." << std::endl;
  }
  virtual std::string Run(const std::string input) {
    std::cout << "    USER: execute " << input << std::endl;
    return std::string("ok: ") + input;
  }
};

#endif  // __PAXOS_BACK_STORE_HH_INCLUDED__
