// -*- C++ -*-
// Automatically generated from include/event.x.
// DO NOT EDIT or your changes may be overwritten

#ifndef __XDR_INCLUDE_EVENT_HH_INCLUDED__
#define __XDR_INCLUDE_EVENT_HH_INCLUDED__ 1

#include <xdrpp/types.h>

using FileName = xdr::xstring<>;

enum ChubbyEvent : std::uint32_t {
  NOP = 0,
  LOCK_CHANGED = 0x100,
  CONTENT_MODIFIED = 0x200,
};
namespace xdr {
template<> struct xdr_traits<::ChubbyEvent>
  : xdr_integral_base<::ChubbyEvent, std::uint32_t> {
  static constexpr bool is_enum = true;
  static constexpr bool is_numeric = false;
  static const char *enum_name(::ChubbyEvent val) {
    switch (val) {
    case ::NOP:
      return "NOP";
    case ::LOCK_CHANGED:
      return "LOCK_CHANGED";
    case ::CONTENT_MODIFIED:
      return "CONTENT_MODIFIED";
    default:
      return nullptr;
    }
  }
};
}

struct EventContent {
  ChubbyEvent event{};
  FileName fname{};
};
namespace xdr {
template<> struct xdr_traits<::EventContent>
  : xdr_struct_base<field_ptr<::EventContent,
                              decltype(::EventContent::event),
                              &::EventContent::event>,
                    field_ptr<::EventContent,
                              decltype(::EventContent::fname),
                              &::EventContent::fname>> {
  template<typename Archive> static void
  save(Archive &ar, const ::EventContent &obj) {
    archive(ar, obj.event, "event");
    archive(ar, obj.fname, "fname");
  }
  template<typename Archive> static void
  load(Archive &ar, ::EventContent &obj) {
    archive(ar, obj.event, "event");
    archive(ar, obj.fname, "fname");
  }
};
}

struct handler_v1 {
  static constexpr std::uint32_t program = 1074036870;
  static constexpr const char *program_name = "event_handler";
  static constexpr std::uint32_t version = 1;
  static constexpr const char *version_name = "handler_v1";

  struct event_callback_t {
    using interface_type = handler_v1;
    static constexpr std::uint32_t proc = 77;
    static constexpr const char *proc_name = "event_callback";
    using arg_type = EventContent;
    using arg_wire_type = EventContent;
    using res_type = void;
    using res_wire_type = xdr::xdr_void;
    
    template<typename C, typename...A> static auto
    dispatch(C &&c, A &&...a) ->
    decltype(c.event_callback(std::forward<A>(a)...)) {
      return c.event_callback(std::forward<A>(a)...);
    }
    
    template<typename C, typename DropIfVoid, typename...A> static auto
    dispatch_dropvoid(C &&c, DropIfVoid &&d, A &&...a) ->
    decltype(c.event_callback(std::forward<DropIfVoid>(d), std::forward<A>(a)...)) {
      return c.event_callback(std::forward<DropIfVoid>(d), std::forward<A>(a)...);
    }
  };

  template<typename T, typename...A> static bool
  call_dispatch(T &&t, std::uint32_t proc, A &&...a) {
    switch(proc) {
    case 77:
      t.template dispatch<event_callback_t>(std::forward<A>(a)...);
      return true;
    }
    return false;
  }

  template<typename _XDRBASE> struct client : _XDRBASE {
    using _XDRBASE::_XDRBASE;

    template<typename..._XDRARGS> auto
    event_callback(_XDRARGS &&..._xdr_args) ->
    decltype(this->_XDRBASE::template invoke<event_callback_t>(_xdr_args...)) {
      return this->_XDRBASE::template invoke<event_callback_t>(_xdr_args...);
    }
  };
};

#endif // !__XDR_INCLUDE_EVENT_HH_INCLUDED__
