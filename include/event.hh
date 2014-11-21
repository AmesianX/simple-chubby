// -*- C++ -*-
// Automatically generated from include/event.x.
// DO NOT EDIT or your changes may be overwritten

#ifndef __XDR_INCLUDE_EVENT_HH_INCLUDED__
#define __XDR_INCLUDE_EVENT_HH_INCLUDED__ 1

#include <xdrpp/types.h>

using eventname = xdr::xstring<>;

struct test_version_event {
  static constexpr std::uint32_t program = 1074036870;
  static constexpr const char *program_name = "test_program_event";
  static constexpr std::uint32_t version = 299;
  static constexpr const char *version_name = "test_version_event";

  struct print_event_t {
    using interface_type = test_version_event;
    static constexpr std::uint32_t proc = 77;
    static constexpr const char *proc_name = "print_event";
    using arg_type = eventname;
    using arg_wire_type = eventname;
    using res_type = void;
    using res_wire_type = xdr::xdr_void;
    
    template<typename C, typename...A> static auto
    dispatch(C &&c, A &&...a) ->
    decltype(c.print_event(std::forward<A>(a)...)) {
      return c.print_event(std::forward<A>(a)...);
    }
    
    template<typename C, typename DropIfVoid, typename...A> static auto
    dispatch_dropvoid(C &&c, DropIfVoid &&d, A &&...a) ->
    decltype(c.print_event(std::forward<DropIfVoid>(d), std::forward<A>(a)...)) {
      return c.print_event(std::forward<DropIfVoid>(d), std::forward<A>(a)...);
    }
  };

  template<typename T, typename...A> static bool
  call_dispatch(T &&t, std::uint32_t proc, A &&...a) {
    switch(proc) {
    case 77:
      t.template dispatch<print_event_t>(std::forward<A>(a)...);
      return true;
    }
    return false;
  }

  template<typename _XDRBASE> struct client : _XDRBASE {
    using _XDRBASE::_XDRBASE;

    template<typename..._XDRARGS> auto
    print_event(_XDRARGS &&..._xdr_args) ->
    decltype(this->_XDRBASE::template invoke<print_event_t>(_xdr_args...)) {
      return this->_XDRBASE::template invoke<print_event_t>(_xdr_args...);
    }
  };
};

#endif // !__XDR_INCLUDE_EVENT_HH_INCLUDED__
