
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

void
Cmd_Echo(int argc, const char *argv[])
{
  int i;

  for (i = 1; i < argc; i++) {
    cout << argv[i] << " ";
  }
  cout << endl;
}

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

void Cmd_FileOpen(int argc, const char *argv[]) {
  if (argc != 3) {
    cout << "Usage: fileOpen FILENAME MODE" << endl;
    return;
  }
  int mode =  strtol(argv[2], 0, 10);
  auto result = client.fileOpen(argv[1],mode);
  cout << result << endl;
}

void Cmd_FileClose(int argc, const char *argv[]) {
  if (argc != 2) {
    cout << "Usage: fileClose FD_ID" << endl;
    return;
  }
  int fd = strtol(argv[1], 0, 10);
  client.fileClose(fd);
}

void Cmd_FileDelete(int argc, const char *argv[])
{
  if (argc != 2) {
    cout << "Usage: fileDelete FD_ID" << endl;
    return;
  }

  try {
    int fd = strtol(argv[1], 0, 10);
    if (client.fileDelete(fd))
      cout << "Deleted" << endl;
    else
      cout << "Failed" << endl;
  } catch (ClientException &e) {
    cout << e.what() << endl;
  }
}


void Cmd_Get(int argc, const char *argv[])
{
  if (argc != 2) {
    cout << "Usage: get FD_ID" << endl;
    return;
  }

  try {
    int fd = strtol(argv[1], 0, 10);
    FileContent content;
    if (client.getContentsAndStat(fd, &content, nullptr))
      cout << content << endl;
    else 
      cout << "Failed" << endl;  
  } catch (ClientException &e) {
    cout << e.what() << endl;
  }
}


void Cmd_Set(int argc, const char *argv[])
{
  if (argc != 3) {
    cout << "Usage: set FD_ID content" << endl;
    return;
  }

  try {
    int fd = strtol(argv[1], 0, 10);
    string content = argv[2];
    if (client.setContents(fd, content))
      cout << "Succeeded" << endl;
    else 
      cout << "Failed" << endl;
  } catch (ClientException &e) {
    cout << e.what() << endl;
  }
}

void Cmd_Acquire(int argc, const char *argv[]) {
  if (argc != 2) {
    cout << "Usage: acquire FD_ID" << endl;
    return;
  }
  try {
    int fd = strtol(argv[1], 0, 10);
    client.acquire(fd);
  } catch (ClientException &e) {
    cout << e.what() << endl;
  }
}


void Cmd_TryAcquire(int argc, const char *argv[]) {
  if (argc != 2) {
    cout << "Usage: tryAcquire FD_ID" << endl;
    return;
  }
  try {
    int fd = strtol(argv[1], 0, 10);
    if(client.tryAcquire(fd))
      cout << "Deleted" << endl;
    else
      cout << "Failed" << endl;
  } catch (ClientException &e) {
    cout << e.what() << endl;
  }
}


void Cmd_Release(int argc, const char *argv[]) {
  if (argc != 2) {
    cout << "Usage: release FD_ID" << endl;
    return;
  }
  try {
    int fd = strtol(argv[1], 0, 10);
    client.release(fd);
  } catch (ClientException &e) {
    cout << e.what() << endl;
  }
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
    } else if (cmd == "fileOpen") {
        Cmd_FileOpen(argc, (const char **)argv);
    } else if (cmd == "fileClose") {
        Cmd_FileClose(argc, (const char **)argv);
    } else if (cmd == "fileDelete") {
        Cmd_FileDelete(argc, (const char **)argv);;
    } else if (cmd == "get") {
        Cmd_Get(argc, (const char **)argv);;
    } else if (cmd == "set") {
        Cmd_Set(argc, (const char **)argv);
    } else if (cmd == "acquire") {
        Cmd_Acquire(argc, (const char **)argv);
    } else if (cmd == "tryAcquire") {
        Cmd_TryAcquire(argc, (const char **)argv);
    } else if (cmd == "release") {
        Cmd_Release(argc, (const char **)argv);
    } else if (cmd == "echo") {
        Cmd_Echo(argc, (const char **)argv);
    } else if (cmd == "exit") {
        exit(0);
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

