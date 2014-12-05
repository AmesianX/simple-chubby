
#include <stdbool.h>
#include <stdint.h>

#include <unistd.h>

#include <string>
#include <iostream>
#include <mutex>

#include "include/client.h"
#include "common.h"

using namespace std;


string membership_parse(string list) {
  string ret = "[ ";
  vector<string> l;
  Tokenize(list, l, "|");

  for (auto& m : l) {
    vector<string> paths;
    Tokenize(m, paths, "/");

    ret += paths.back() + " ";
  }

  ret += "]";
  return ret;
}

int main(int argc, const char *argv[]) {

  Client client = Client();

  if (argc < 3) {
    cout << "Usage: " << argv[0] << " HOST MY_ID [RUN PERIOD in seconds]" << endl;
    return 1;
  }

  string host = argv[1];
  string myId = argv[2];
  uint32_t run = (uint32_t)-1;
  if (argc >= 4) run = stoul(argv[3]);

  // Setup connection
  try {
    client.open(host);
  } catch (exception &e) {
    cout << "Connection failed!" << endl;
    cout << "Exception: " << e.what() << endl;
    return 1;
  }

  mutex lk;
  string grouplist;
  const string groupdir = "/group";

  try {
    FileHandlerId dirId;
    FileHandlerId fdId;

    // event callback
    auto renew = [&](string fname) {
      if (fname == groupdir) {
        FileContent newlist;
        checkErr(client.getContentsAndStat(dirId, &newlist, nullptr),
                 "get content fails!");
        if (newlist != grouplist) {
          {
            lock_guard<mutex> lock(lk);
            grouplist = newlist;
          }
          info("group membership changed");
        }
      }
    };

    // create directory if doesn't exist
    dirId = client.fileOpen(groupdir, CREATE_DIRECTORY);
    if (dirId != FAIL) client.fileClose(dirId);

    checkErr((dirId = client.fileOpen(groupdir, READ | WRITE | EV_CONTENT_MODIFIED)) != FAIL,
             "open " + groupdir + " fails!");

    client.registerCallback(ChubbyEvent::CONTENT_MODIFIED, renew);

    // create file
    string myFile = groupdir + "/" + myId;
    checkErr((fdId = client.fileOpen(myFile, CREATE_FILE | EPHEMERAL)) != FAIL,
             "create ephemeral file " + myId + " fails!");

    renew(groupdir);

    while(run--) {
      string curlist;
      {
        lock_guard<mutex> lock(lk);
        curlist = grouplist;
      }
      info("current membership is " + membership_parse(curlist));
      sleep(1);
    }

    checkErr(client.fileDelete(fdId), "delete file fails!");
    client.deleteCallback(ChubbyEvent::CONTENT_MODIFIED);
    client.fileClose(dirId);
    client.fileClose(fdId);

  } catch(exception &e) {
    cout << e.what() << endl;
    return 1;
  }

  client.close();

  return 0;
}


