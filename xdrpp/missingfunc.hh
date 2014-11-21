#include <stdlib.h>
#include <stdio.h>
#include <string>

#ifndef _MISSINGFUNC_HH_
#define _MISSINGFUNC_HH_ 1


/*
 * define some missing functions in std library
 */
namespace std {

#ifdef __CYGWIN__
  inline string to_string (int val)
  {
    char buffer [12];
    sprintf (buffer, "%d", val);
    return string(buffer);
  }
  
  
  inline int stol (const string&  str, size_t* idx = 0, int base = 10)
  {
    const char *s = str.c_str();
    char *endptr;
    long val = strtol(s, &endptr, base);
    *idx = endptr - s;
    return val;
  }
  
  inline int stoi (const string&  str, size_t* idx = 0, int base = 10)
  {
    return (int) stol(str, idx, base);
  }
  
  inline long long stoll (const string&  str, size_t* idx = 0, int base = 10)
  {
    const char *s = str.c_str();
    char *endptr;
    long val = strtoll(s, &endptr, base);
    *idx = endptr - s;
    return val;
  }

  inline unsigned long stoul (const string&  str, size_t* idx = 0, int base = 10)
  {
    const char *s = str.c_str();
    char *endptr;
    unsigned long val = strtoul(s, &endptr, base);
    *idx = endptr - s;
    return val;
  }

  inline long double stold (const string&  str, size_t* idx = 0){
    const char *s = str.c_str();
    char *endptr;
    long double val = strtold(s, &endptr);
    *idx = endptr - s;
    return val;
  }
  
  inline double stod (const string&  str, size_t* idx = 0)
  {
    const char *s = str.c_str();
    char *endptr;
    double val = strtod(s, &endptr);
    *idx = endptr - s;
    return val;
  }
  

  inline float stof (const string&  str, size_t* idx = 0)
  {
    const char *s = str.c_str();
    char *endptr;
    float val = strtof(s, &endptr);
    *idx = endptr - s;
    return val;
  }
#endif
}


#endif
