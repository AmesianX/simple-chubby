#include <thread>
#include <iostream>
#include "paxos/helper.hh"
#include "paxos/back_store.hh"
#include "paxos/paxos_lib.hh"

int main(int argc, char* argv[]) {
  if (argc != 3) {
    std::cout << "./paxos_replica [config_file_name]"
        << "[replica_rank_start_from_0]" << std::endl
        << "config file format:" << std::endl
        << "quota; number_of_clients; "
        << "replica_address; client_address ..." << std::endl;
    return 1;
  }

  BackStore back_store;
  PaxosLib paxos_lib(argv[1], std::stoi(argv[2]), &back_store);
  std::thread run_thread(std::bind(&PaxosLib::Run, &paxos_lib));

  std::string line;
  while (true) {
    std::getline(std::cin, line);
    /*
    struct execute_arg {
      cid_t cid;
      uid_t rid;
      viewid_t vid;
      opaque request<>;
    };
    */
    std::unique_ptr<execute_arg> arg(new execute_arg);
    arg->request.append((const unsigned char*)line.c_str(), line.size());
    std::unique_ptr<execute_res> result =
        paxos_lib.paxos_interface_for_user->execute(std::move(arg));
    if (result->ok()) {
      std::cout << "Reply: " << OpaqueToString(result->reply()) << std::endl;
    } else {
      std::cout << "Not the leader." << std::endl;
    }
  }

  run_thread.join();
  return 0;
}
