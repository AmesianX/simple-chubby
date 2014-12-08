
#include <stdbool.h>
#include <stdint.h>

#include <unistd.h>

#include <string>
#include <iostream>
#include <mutex>
#include <condition_variable>

#include "include/client.h"
#include "common.h"

using namespace std;

class chubby_dbar {
private:
  uint32_t num;
  const string name;
  bool inRegion;

  Client *c;
  string fcount;
  string fpassed;
  FileHandlerId countfdId;
  FileHandlerId passedfdId;

  bool passed;
  mutex lk;
  condition_variable cv;

public:
  chubby_dbar(uint32_t n, const string &host, const string &nm = "bar")
      : num(n), name(nm), inRegion(false),
        c(new Client()), fcount("/"+name+"count"), fpassed("/"+name+"passed"),
        passed(false) {

    // Setup connection
    try {
      c->open(host);
    } catch (exception &e) {
      cout << "Connection failed!" << endl;
      cout << "Exception: " << e.what() << endl;
      return;
    }

    auto callback = [this](string fname) {
      if (fname == fpassed) {
        {
          lock_guard<mutex> lock(lk);
          passed = true;
        }
        cv.notify_all();
      }
    };

    c->registerCallback(ChubbyEvent::CONTENT_MODIFIED, callback);

    // create files if don't exist
    auto fdId = c->fileOpen(fcount, CREATE_FILE);
    if (fdId != FAIL) c->fileClose(fdId);
    fdId = c->fileOpen(fpassed, CREATE_FILE);
    if (fdId != FAIL) c->fileClose(fdId);

    // open files
    checkErr((countfdId = c->fileOpen(fcount, READ | WRITE)) != FAIL,
             "open " + fcount + " fails!");
    checkErr((passedfdId = c->fileOpen(fpassed, READ | WRITE | EV_CONTENT_MODIFIED)) != FAIL,
             "open " + fpassed + " fails!");
  }

  ~chubby_dbar() {
    c->fileClose(countfdId);
    c->fileClose(passedfdId);

    c->close();
    delete c;
  }

  void enter() { sync(true); }
  void leave() { sync(false); }

private:
  void sync(bool isEnter) {

    c->acquire(countfdId);

    // read and set number of processes reaching the barrier
    string numInBar;
    checkErr(c->getContentsAndStat(countfdId, (FileContent*)&numInBar, nullptr),
             "get content fails!");
    if (numInBar.empty())
      numInBar = "1";
    else
      numInBar = to_string(stoul(numInBar) + 1);
    checkErr(c->setContents(countfdId, numInBar),
             "set cotent fails!");

    // check if passing the barrier
    if (numInBar == to_string(num)) {
      // clear \c count file
      checkErr(c->setContents(countfdId, ""), "set content fails!");
      // touch \c passed file to trigger leaving of barrier
      checkErr(c->setContents(passedfdId, ""), "set content fails!");

      inRegion = isEnter;
    }

    c->release(countfdId);

    // wait
    {
      unique_lock<mutex> lock(lk);
      cv.wait(lock, [this](){ return passed; });
      passed = false;
    }

  }

};

int main(int argc, const char *argv[]) {

  if (argc != 4) {
    cout << "Usage: " << argv[0] << " HOST MY_ID NUM" << endl;
    return 1;
  }

  string host = argv[1];
  string myId = argv[2];
  uint32_t num = stoul(argv[3]);

  chubby_dbar bar(num, host);

  info(myId + " wait to enter");
  bar.enter();
  info(myId + " entered");

  char c;
  cin >> c;

  info(myId + " wait to leave");
  bar.leave();
  info(myId + " left");

  return 0;
}


