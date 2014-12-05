
#ifndef __APPS_COMMON_H__
#define __APPS_COMMON_H__

#include <unistd.h>
#include <cstdlib>
#include <iostream>
#include <string>
#include "include/client.h"

void checkErr(bool ret, std::string err_msg) {
  if (!ret) {
    std::cerr << "[APP ERROR]: " << err_msg << std::endl;
    std::exit(1);
  }
}

void info(std::string msg) {
  std::cout << "[APP]: " << msg << std::endl;
}

#endif // __APPS_COMMON_H__
