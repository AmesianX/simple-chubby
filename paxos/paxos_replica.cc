#include <thread>
#include <iostream>
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

  PaxosLib paxos_lib(argv[1], std::stoi(argv[2]));
  std::thread run_thread(std::bind(&PaxosLib::Run, &paxos_lib));

  std::string line;
  while (true) {
    std::getline(std::cin, line);
    paxos_lib.execute_replicate_engine->replicateCommand(line);
  }

  run_thread.join();
  return 0;
}
