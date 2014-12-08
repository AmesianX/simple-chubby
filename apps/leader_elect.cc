
#include <stdbool.h>
#include <stdint.h>

#include <unistd.h>

#include <string>
#include <iostream>
#include <mutex>

#include "include/client.h"
#include "common.h"

using namespace std;


int main(int argc, const char *argv[]) {

  Client client = Client();

  if (argc != 3) {
    cout << "Usage: " << argv[0] << " HOST MY_ID" << endl;
    return 1;
  }

  string host = argv[1];
  string myId = argv[2];
  const string leaderfile = "/leader";

  // Setup connection
  try {
    client.open(host);
  } catch (exception &e) {
    cout << "Connection failed!" << endl;
    cout << "Exception: " << e.what() << endl;
    return 1;
  }

  mutex lk;
  string leader;


  try {
    FileHandlerId fdId;

    // event callback
    auto compete = [&](string fname) {
      if (fname == leaderfile) {
        if (client.tryAcquire(fdId)) {
          // get lock and become leader
          leader = myId;
          checkErr(client.setContents(fdId, leader), "set content fails!");
          info("set " + leader + " as the leader");
        }
      }
    };
    auto renew = [&](string fname) {
      if (fname == leaderfile) {
        FileContent newleader;
        checkErr(client.getContentsAndStat(fdId, &newleader, nullptr),
                 "get content fails!");
        if (newleader != leader) {
          {
            lock_guard<mutex> lock(lk);
            leader = newleader;
          }
          info("find out current leader is " + newleader);
        }
      }
    };

    // create file if doesn't exist
    fdId = client.fileOpen(leaderfile, CREATE_FILE);
    if (fdId != FAIL) client.fileClose(fdId);

    client.registerCallback(ChubbyEvent::LOCK_CHANGED, compete);
    client.registerCallback(ChubbyEvent::CONTENT_MODIFIED, renew);

    // open file
    checkErr((fdId = client.fileOpen(leaderfile, READ | WRITE | EV_LOCK_CHANGED | EV_CONTENT_MODIFIED)) != FAIL,
             "open file " + leaderfile + " fails!");

    compete(leaderfile);

    renew(leaderfile);

    string lastleader = "";
    while(true) {
      string curleader;
      {
        lock_guard<mutex> lock(lk);
        curleader = leader;
      }
      if (curleader == lastleader) {
        std::cout << ".";
        std::cout.flush();
      } else {
        info("current leader is " + curleader);
      }
      lastleader = curleader;
      sleep(1);
    }

  } catch(exception &e) {
    cout << e.what() << endl;
    return 1;
  }

  client.close();

  return 0;
}


