// -*- C++ -*-
// Automatically generated from include/server.x.
// DO NOT EDIT or your changes may be overwritten

#ifndef __XDR_INCLUDE_SERVER_HH_INCLUDED__
#define __XDR_INCLUDE_SERVER_HH_INCLUDED__ 1

#include <xdrpp/types.h>

struct test_version {
  static constexpr std::uint32_t program = 1074036870;
  static constexpr const char *program_name = "test_program";
  static constexpr std::uint32_t version = 199;
  static constexpr const char *version_name = "test_version";

  struct increment_t {
    using interface_type = test_version;
    static constexpr std::uint32_t proc = 66;
    static constexpr const char *proc_name = "increment";
    using arg_type = int;
    using arg_wire_type = int;
    using res_type = int;
    using res_wire_type = int;
    
    template<typename C, typename...A> static auto
    dispatch(C &&c, A &&...a) ->
    decltype(c.increment(std::forward<A>(a)...)) {
      return c.increment(std::forward<A>(a)...);
    }
    
    template<typename C, typename DropIfVoid, typename...A> static auto
    dispatch_dropvoid(C &&c, DropIfVoid &&d, A &&...a) ->
    decltype(c.increment(std::forward<DropIfVoid>(d), std::forward<A>(a)...)) {
      return c.increment(std::forward<DropIfVoid>(d), std::forward<A>(a)...);
    }
  };

  struct decrement_t {
    using interface_type = test_version;
    static constexpr std::uint32_t proc = 88;
    static constexpr const char *proc_name = "decrement";
    using arg_type = int;
    using arg_wire_type = int;
    using res_type = int;
    using res_wire_type = int;
    
    template<typename C, typename...A> static auto
    dispatch(C &&c, A &&...a) ->
    decltype(c.decrement(std::forward<A>(a)...)) {
      return c.decrement(std::forward<A>(a)...);
    }
    
    template<typename C, typename DropIfVoid, typename...A> static auto
    dispatch_dropvoid(C &&c, DropIfVoid &&d, A &&...a) ->
    decltype(c.decrement(std::forward<DropIfVoid>(d), std::forward<A>(a)...)) {
      return c.decrement(std::forward<DropIfVoid>(d), std::forward<A>(a)...);
    }
  };

  template<typename T, typename...A> static bool
  call_dispatch(T &&t, std::uint32_t proc, A &&...a) {
    switch(proc) {
    case 66:
      t.template dispatch<increment_t>(std::forward<A>(a)...);
      return true;
    case 88:
      t.template dispatch<decrement_t>(std::forward<A>(a)...);
      return true;
    }
    return false;
  }

  template<typename _XDRBASE> struct client : _XDRBASE {
    using _XDRBASE::_XDRBASE;

    template<typename..._XDRARGS> auto
    increment(_XDRARGS &&..._xdr_args) ->
    decltype(this->_XDRBASE::template invoke<increment_t>(_xdr_args...)) {
      return this->_XDRBASE::template invoke<increment_t>(_xdr_args...);
    }

    template<typename..._XDRARGS> auto
    decrement(_XDRARGS &&..._xdr_args) ->
    decltype(this->_XDRBASE::template invoke<decrement_t>(_xdr_args...)) {
      return this->_XDRBASE::template invoke<decrement_t>(_xdr_args...);
    }
  };
};

#endif // !__XDR_INCLUDE_SERVER_HH_INCLUDED__
