
#include <stdbool.h>
#include <stdint.h>

#include <unistd.h>

#include <string>
#include <fstream>
#include <iostream>
#include <thread>
#include <functional>

#include <xdrpp/srpc.h>
#include <server/chubby_client.h>

#include <include/server.hh>
#include <include/client.h>

using namespace std;

Client client;

#define DEBUG_MAX_ARGS      5
#define DEBUG_MAX_LINE      256

void Cmd_Increment(int argc, const char *argv[]) {
  if (argc != 2) {
    cout << "Usage: increment INT" << endl;
    return;
  }
  auto result = client.increment(std::stoi(argv[1]));
  cout << result << endl;
}

void Cmd_Decrement(int argc, const char *argv[]) {
  if (argc != 2) {
    cout << "Usage: decrement INT" << endl;
    return;
  }
  auto result = client.decrement(std::stoi(argv[1]));
  cout << result << endl;
}

void
DispatchCommand(char *buf)
{
    int argc;
    char *argv[DEBUG_MAX_ARGS];
    char *nextArg;

    // parse input
    argv[0] = buf;
    for (argc = 1; argc < DEBUG_MAX_ARGS; argc++) {
        nextArg = strchr(argv[argc - 1], ' ');
        if (nextArg == NULL)
        {
            break;
        }

        *nextArg = '\0';
        argv[argc] = nextArg + 1;
    }

    // execute command
    string cmd = argv[0];
    if (cmd == "increment") {
        Cmd_Increment(argc, (const char **)argv);
    } else if (cmd == "decrement") {
        Cmd_Decrement(argc, (const char **)argv);
    } else if (cmd == "#") {
        // Ignore comments
    } else if (cmd != "") {
        printf("Unknown command '%s'\n", argv[0]);
    }
}

void
Prompt()
{
    char buf[DEBUG_MAX_LINE];
    cout << "Client Shell" << endl;

    while (cin.good()) {
        cout << "> ";

        // read input
        cin.getline((char *)&buf, DEBUG_MAX_LINE);

        DispatchCommand(buf);
    }
}

void
RunScript(const char *file)
{
    char buf[DEBUG_MAX_LINE];
    fstream fin {file};

    while (!fin.eof()) {
        // read input
        fin.getline((char *)&buf, DEBUG_MAX_LINE);

        DispatchCommand(buf);
    }
}

void keepGetEvent(Client* client) {
  while (true) {
    client->getEvent();
  }
}

int
main(int argc, const char *argv[])
{
    client = Client();

    if (argc != 2 && argc != 3) {
        cout << "Usage: shell HOST [SCRIPT]" << endl;
        return 1;
    }

    // Setup connection
    try {
        client.open(argv[1]);
    } catch (exception &e) {
        cout << "Connection failed!" << endl;
        cout << "Exception: " << e.what() << endl;
        return 1;
    }

    std::thread event_thread(std::bind(keepGetEvent, &client));
    event_thread.detach();

    // Either execute script or prompt
    try {
        if (argc == 2) {
            Prompt();
        } else {
            RunScript(argv[2]);
        }
    } catch(exception &e) {
        cout << e.what() << endl;
        return 1;
    }

    return 0;
}

