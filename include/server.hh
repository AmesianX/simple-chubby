// -*- C++ -*-
// Automatically generated from include/server.x.
// DO NOT EDIT or your changes may be overwritten

#ifndef __XDR_INCLUDE_SERVER_HH_INCLUDED__
#define __XDR_INCLUDE_SERVER_HH_INCLUDED__ 1

#include <xdrpp/types.h>

using longstring = xdr::xstring<>;
using ErrorCode = std::uint32_t;
using SetString = xdr::xvector<longstring>;

struct RPCBool {
private:
  std::uint32_t discriminant_;
  union {
    bool val_;
    ErrorCode errCode_;
  };

public:
  static_assert (sizeof (unsigned) <= 4, "union discriminant must be 4 bytes");

  static constexpr int _xdr_field_number(std::uint32_t which) {
    return which == 0 ? 1
      : which == 1 ? 2
      : 0;
  }
  template<typename _F, typename...A> static bool
  _xdr_with_mem_ptr(_F &_f, std::uint32_t which, A&&...a) {
    switch (which) {
    case 0:
      _f(&RPCBool::val_, std::forward<A>(a)...);
      return true;
    case 1:
      _f(&RPCBool::errCode_, std::forward<A>(a)...);
      return true;
    default:
      return true;
    }
  }

  std::uint32_t _xdr_discriminant() const { return discriminant_; }
  void _xdr_discriminant(std::uint32_t which, bool validate = true) {
    int fnum = _xdr_field_number(which);
    if (fnum < 0 && validate)
      throw xdr::xdr_bad_discriminant("bad value of discriminant in RPCBool");
    if (fnum != _xdr_field_number(discriminant_)) {
      this->~RPCBool();
      discriminant_ = which;
      _xdr_with_mem_ptr(xdr::field_constructor, discriminant_, *this);
    }
  }
  RPCBool(std::uint32_t which = std::uint32_t{}) : discriminant_(which) {
    _xdr_with_mem_ptr(xdr::field_constructor, discriminant_, *this);
  }
  RPCBool(const RPCBool &source) : discriminant_(source.discriminant_) {
    _xdr_with_mem_ptr(xdr::field_constructor, discriminant_, *this, source);
  }
  RPCBool(RPCBool &&source) : discriminant_(source.discriminant_) {
    _xdr_with_mem_ptr(xdr::field_constructor, discriminant_, *this,
                      std::move(source));
  }
  ~RPCBool() { _xdr_with_mem_ptr(xdr::field_destructor, discriminant_, *this); }
  RPCBool &operator=(const RPCBool &source) {
    if (_xdr_field_number(discriminant_) 
        == _xdr_field_number(source.discriminant_))
      _xdr_with_mem_ptr(xdr::field_assigner, discriminant_, *this, source);
    else {
      this->~RPCBool();
      discriminant_ = std::uint32_t(-1);
      _xdr_with_mem_ptr(xdr::field_constructor, discriminant_, *this, source);
    }
    discriminant_ = source.discriminant_;
    return *this;
  }
  RPCBool &operator=(RPCBool &&source) {
    if (_xdr_field_number(discriminant_)
         == _xdr_field_number(source.discriminant_))
      _xdr_with_mem_ptr(xdr::field_assigner, discriminant_, *this,
                        std::move(source));
    else {
      this->~RPCBool();
      discriminant_ = std::uint32_t(-1);
      _xdr_with_mem_ptr(xdr::field_constructor, discriminant_, *this,
                        std::move(source));
    }
    discriminant_ = source.discriminant_;
    return *this;
  }

  std::uint32_t discriminant() const { return std::uint32_t(discriminant_); }
  RPCBool &discriminant(unsigned _xdr_d, bool _xdr_validate = true) {
    _xdr_discriminant(_xdr_d, _xdr_validate);
    return *this;
  }

  bool &val() {
    if (_xdr_field_number(discriminant_) == 1)
      return val_;
    throw xdr::xdr_wrong_union("RPCBool: val accessed when not selected");
  }
  const bool &val() const {
    if (_xdr_field_number(discriminant_) == 1)
      return val_;
    throw xdr::xdr_wrong_union("RPCBool: val accessed when not selected");
  }
  ErrorCode &errCode() {
    if (_xdr_field_number(discriminant_) == 2)
      return errCode_;
    throw xdr::xdr_wrong_union("RPCBool: errCode accessed when not selected");
  }
  const ErrorCode &errCode() const {
    if (_xdr_field_number(discriminant_) == 2)
      return errCode_;
    throw xdr::xdr_wrong_union("RPCBool: errCode accessed when not selected");
  }
};
namespace xdr {
template<> struct xdr_traits<::RPCBool> : xdr_traits_base {
  static constexpr bool is_class = true;
  static constexpr bool is_union = true;
  static constexpr bool has_fixed_size = false;

  using union_type = ::RPCBool;
  using discriminant_type = decltype(std::declval<union_type>().discriminant());

  static constexpr const char *union_field_name(std::uint32_t which) {
    return which == 0 ? "val"
      : which == 1 ? "errCode"
      : nullptr;
  }
  static const char *union_field_name(const union_type &u) {
    return union_field_name(u._xdr_discriminant());
  }

  static std::size_t serial_size(const ::RPCBool &obj) {
    std::size_t size = 0;
    if (!obj._xdr_with_mem_ptr(field_size, obj._xdr_discriminant(), obj, size))
      throw xdr_bad_discriminant("bad value of discriminant in RPCBool");
    return size + 4;
  }
  template<typename Archive> static void
  save(Archive &ar, const ::RPCBool &obj) {
    xdr::archive(ar, obj.discriminant(), "discriminant");
    if (!obj._xdr_with_mem_ptr(field_archiver, obj.discriminant(), ar, obj,
                               union_field_name(obj)))
      throw xdr_bad_discriminant("bad value of discriminant in RPCBool");
  }
  template<typename Archive> static void
  load(Archive &ar, ::RPCBool &obj) {
    discriminant_type which;
    xdr::archive(ar, which, "discriminant");
    obj.discriminant(which);
    obj._xdr_with_mem_ptr(field_archiver, obj.discriminant(), ar, obj,
                          union_field_name(which));
  }
};
}

struct RPCString {
private:
  std::uint32_t discriminant_;
  union {
    xdr::xstring<> val_;
    ErrorCode errCode_;
  };

public:
  static_assert (sizeof (unsigned) <= 4, "union discriminant must be 4 bytes");

  static constexpr int _xdr_field_number(std::uint32_t which) {
    return which == 0 ? 1
      : which == 1 ? 2
      : 0;
  }
  template<typename _F, typename...A> static bool
  _xdr_with_mem_ptr(_F &_f, std::uint32_t which, A&&...a) {
    switch (which) {
    case 0:
      _f(&RPCString::val_, std::forward<A>(a)...);
      return true;
    case 1:
      _f(&RPCString::errCode_, std::forward<A>(a)...);
      return true;
    default:
      return true;
    }
  }

  std::uint32_t _xdr_discriminant() const { return discriminant_; }
  void _xdr_discriminant(std::uint32_t which, bool validate = true) {
    int fnum = _xdr_field_number(which);
    if (fnum < 0 && validate)
      throw xdr::xdr_bad_discriminant("bad value of discriminant in RPCString");
    if (fnum != _xdr_field_number(discriminant_)) {
      this->~RPCString();
      discriminant_ = which;
      _xdr_with_mem_ptr(xdr::field_constructor, discriminant_, *this);
    }
  }
  RPCString(std::uint32_t which = std::uint32_t{}) : discriminant_(which) {
    _xdr_with_mem_ptr(xdr::field_constructor, discriminant_, *this);
  }
  RPCString(const RPCString &source) : discriminant_(source.discriminant_) {
    _xdr_with_mem_ptr(xdr::field_constructor, discriminant_, *this, source);
  }
  RPCString(RPCString &&source) : discriminant_(source.discriminant_) {
    _xdr_with_mem_ptr(xdr::field_constructor, discriminant_, *this,
                      std::move(source));
  }
  ~RPCString() { _xdr_with_mem_ptr(xdr::field_destructor, discriminant_, *this); }
  RPCString &operator=(const RPCString &source) {
    if (_xdr_field_number(discriminant_) 
        == _xdr_field_number(source.discriminant_))
      _xdr_with_mem_ptr(xdr::field_assigner, discriminant_, *this, source);
    else {
      this->~RPCString();
      discriminant_ = std::uint32_t(-1);
      _xdr_with_mem_ptr(xdr::field_constructor, discriminant_, *this, source);
    }
    discriminant_ = source.discriminant_;
    return *this;
  }
  RPCString &operator=(RPCString &&source) {
    if (_xdr_field_number(discriminant_)
         == _xdr_field_number(source.discriminant_))
      _xdr_with_mem_ptr(xdr::field_assigner, discriminant_, *this,
                        std::move(source));
    else {
      this->~RPCString();
      discriminant_ = std::uint32_t(-1);
      _xdr_with_mem_ptr(xdr::field_constructor, discriminant_, *this,
                        std::move(source));
    }
    discriminant_ = source.discriminant_;
    return *this;
  }

  std::uint32_t discriminant() const { return std::uint32_t(discriminant_); }
  RPCString &discriminant(unsigned _xdr_d, bool _xdr_validate = true) {
    _xdr_discriminant(_xdr_d, _xdr_validate);
    return *this;
  }

  xdr::xstring<> &val() {
    if (_xdr_field_number(discriminant_) == 1)
      return val_;
    throw xdr::xdr_wrong_union("RPCString: val accessed when not selected");
  }
  const xdr::xstring<> &val() const {
    if (_xdr_field_number(discriminant_) == 1)
      return val_;
    throw xdr::xdr_wrong_union("RPCString: val accessed when not selected");
  }
  ErrorCode &errCode() {
    if (_xdr_field_number(discriminant_) == 2)
      return errCode_;
    throw xdr::xdr_wrong_union("RPCString: errCode accessed when not selected");
  }
  const ErrorCode &errCode() const {
    if (_xdr_field_number(discriminant_) == 2)
      return errCode_;
    throw xdr::xdr_wrong_union("RPCString: errCode accessed when not selected");
  }
};
namespace xdr {
template<> struct xdr_traits<::RPCString> : xdr_traits_base {
  static constexpr bool is_class = true;
  static constexpr bool is_union = true;
  static constexpr bool has_fixed_size = false;

  using union_type = ::RPCString;
  using discriminant_type = decltype(std::declval<union_type>().discriminant());

  static constexpr const char *union_field_name(std::uint32_t which) {
    return which == 0 ? "val"
      : which == 1 ? "errCode"
      : nullptr;
  }
  static const char *union_field_name(const union_type &u) {
    return union_field_name(u._xdr_discriminant());
  }

  static std::size_t serial_size(const ::RPCString &obj) {
    std::size_t size = 0;
    if (!obj._xdr_with_mem_ptr(field_size, obj._xdr_discriminant(), obj, size))
      throw xdr_bad_discriminant("bad value of discriminant in RPCString");
    return size + 4;
  }
  template<typename Archive> static void
  save(Archive &ar, const ::RPCString &obj) {
    xdr::archive(ar, obj.discriminant(), "discriminant");
    if (!obj._xdr_with_mem_ptr(field_archiver, obj.discriminant(), ar, obj,
                               union_field_name(obj)))
      throw xdr_bad_discriminant("bad value of discriminant in RPCString");
  }
  template<typename Archive> static void
  load(Archive &ar, ::RPCString &obj) {
    discriminant_type which;
    xdr::archive(ar, which, "discriminant");
    obj.discriminant(which);
    obj._xdr_with_mem_ptr(field_archiver, obj.discriminant(), ar, obj,
                          union_field_name(which));
  }
};
}

struct RPCSet {
private:
  std::uint32_t discriminant_;
  union {
    SetString val_;
    ErrorCode errCode_;
  };

public:
  static_assert (sizeof (unsigned) <= 4, "union discriminant must be 4 bytes");

  static constexpr int _xdr_field_number(std::uint32_t which) {
    return which == 0 ? 1
      : which == 1 ? 2
      : 0;
  }
  template<typename _F, typename...A> static bool
  _xdr_with_mem_ptr(_F &_f, std::uint32_t which, A&&...a) {
    switch (which) {
    case 0:
      _f(&RPCSet::val_, std::forward<A>(a)...);
      return true;
    case 1:
      _f(&RPCSet::errCode_, std::forward<A>(a)...);
      return true;
    default:
      return true;
    }
  }

  std::uint32_t _xdr_discriminant() const { return discriminant_; }
  void _xdr_discriminant(std::uint32_t which, bool validate = true) {
    int fnum = _xdr_field_number(which);
    if (fnum < 0 && validate)
      throw xdr::xdr_bad_discriminant("bad value of discriminant in RPCSet");
    if (fnum != _xdr_field_number(discriminant_)) {
      this->~RPCSet();
      discriminant_ = which;
      _xdr_with_mem_ptr(xdr::field_constructor, discriminant_, *this);
    }
  }
  RPCSet(std::uint32_t which = std::uint32_t{}) : discriminant_(which) {
    _xdr_with_mem_ptr(xdr::field_constructor, discriminant_, *this);
  }
  RPCSet(const RPCSet &source) : discriminant_(source.discriminant_) {
    _xdr_with_mem_ptr(xdr::field_constructor, discriminant_, *this, source);
  }
  RPCSet(RPCSet &&source) : discriminant_(source.discriminant_) {
    _xdr_with_mem_ptr(xdr::field_constructor, discriminant_, *this,
                      std::move(source));
  }
  ~RPCSet() { _xdr_with_mem_ptr(xdr::field_destructor, discriminant_, *this); }
  RPCSet &operator=(const RPCSet &source) {
    if (_xdr_field_number(discriminant_) 
        == _xdr_field_number(source.discriminant_))
      _xdr_with_mem_ptr(xdr::field_assigner, discriminant_, *this, source);
    else {
      this->~RPCSet();
      discriminant_ = std::uint32_t(-1);
      _xdr_with_mem_ptr(xdr::field_constructor, discriminant_, *this, source);
    }
    discriminant_ = source.discriminant_;
    return *this;
  }
  RPCSet &operator=(RPCSet &&source) {
    if (_xdr_field_number(discriminant_)
         == _xdr_field_number(source.discriminant_))
      _xdr_with_mem_ptr(xdr::field_assigner, discriminant_, *this,
                        std::move(source));
    else {
      this->~RPCSet();
      discriminant_ = std::uint32_t(-1);
      _xdr_with_mem_ptr(xdr::field_constructor, discriminant_, *this,
                        std::move(source));
    }
    discriminant_ = source.discriminant_;
    return *this;
  }

  std::uint32_t discriminant() const { return std::uint32_t(discriminant_); }
  RPCSet &discriminant(unsigned _xdr_d, bool _xdr_validate = true) {
    _xdr_discriminant(_xdr_d, _xdr_validate);
    return *this;
  }

  SetString &val() {
    if (_xdr_field_number(discriminant_) == 1)
      return val_;
    throw xdr::xdr_wrong_union("RPCSet: val accessed when not selected");
  }
  const SetString &val() const {
    if (_xdr_field_number(discriminant_) == 1)
      return val_;
    throw xdr::xdr_wrong_union("RPCSet: val accessed when not selected");
  }
  ErrorCode &errCode() {
    if (_xdr_field_number(discriminant_) == 2)
      return errCode_;
    throw xdr::xdr_wrong_union("RPCSet: errCode accessed when not selected");
  }
  const ErrorCode &errCode() const {
    if (_xdr_field_number(discriminant_) == 2)
      return errCode_;
    throw xdr::xdr_wrong_union("RPCSet: errCode accessed when not selected");
  }
};
namespace xdr {
template<> struct xdr_traits<::RPCSet> : xdr_traits_base {
  static constexpr bool is_class = true;
  static constexpr bool is_union = true;
  static constexpr bool has_fixed_size = false;

  using union_type = ::RPCSet;
  using discriminant_type = decltype(std::declval<union_type>().discriminant());

  static constexpr const char *union_field_name(std::uint32_t which) {
    return which == 0 ? "val"
      : which == 1 ? "errCode"
      : nullptr;
  }
  static const char *union_field_name(const union_type &u) {
    return union_field_name(u._xdr_discriminant());
  }

  static std::size_t serial_size(const ::RPCSet &obj) {
    std::size_t size = 0;
    if (!obj._xdr_with_mem_ptr(field_size, obj._xdr_discriminant(), obj, size))
      throw xdr_bad_discriminant("bad value of discriminant in RPCSet");
    return size + 4;
  }
  template<typename Archive> static void
  save(Archive &ar, const ::RPCSet &obj) {
    xdr::archive(ar, obj.discriminant(), "discriminant");
    if (!obj._xdr_with_mem_ptr(field_archiver, obj.discriminant(), ar, obj,
                               union_field_name(obj)))
      throw xdr_bad_discriminant("bad value of discriminant in RPCSet");
  }
  template<typename Archive> static void
  load(Archive &ar, ::RPCSet &obj) {
    discriminant_type which;
    xdr::archive(ar, which, "discriminant");
    obj.discriminant(which);
    obj._xdr_with_mem_ptr(field_archiver, obj.discriminant(), ar, obj,
                          union_field_name(which));
  }
};
}

struct kvpair {
  xdr::xstring<512> key{};
  xdr::xstring<> val{};
};
namespace xdr {
template<> struct xdr_traits<::kvpair>
  : xdr_struct_base<field_ptr<::kvpair,
                              decltype(::kvpair::key),
                              &::kvpair::key>,
                    field_ptr<::kvpair,
                              decltype(::kvpair::val),
                              &::kvpair::val>> {
  template<typename Archive> static void
  save(Archive &ar, const ::kvpair &obj) {
    archive(ar, obj.key, "key");
    archive(ar, obj.val, "val");
  }
  template<typename Archive> static void
  load(Archive &ar, ::kvpair &obj) {
    archive(ar, obj.key, "key");
    archive(ar, obj.val, "val");
  }
};
}

struct api_v1 {
  static constexpr std::uint32_t program = 1074036870;
  static constexpr const char *program_name = "server_api";
  static constexpr std::uint32_t version = 1;
  static constexpr const char *version_name = "api_v1";

  struct create_t {
    using interface_type = api_v1;
    static constexpr std::uint32_t proc = 1;
    static constexpr const char *proc_name = "create";
    using arg_type = kvpair;
    using arg_wire_type = kvpair;
    using res_type = RPCBool;
    using res_wire_type = RPCBool;
    
    template<typename C, typename...A> static auto
    dispatch(C &&c, A &&...a) ->
    decltype(c.create(std::forward<A>(a)...)) {
      return c.create(std::forward<A>(a)...);
    }
    
    template<typename C, typename DropIfVoid, typename...A> static auto
    dispatch_dropvoid(C &&c, DropIfVoid &&d, A &&...a) ->
    decltype(c.create(std::forward<DropIfVoid>(d), std::forward<A>(a)...)) {
      return c.create(std::forward<DropIfVoid>(d), std::forward<A>(a)...);
    }
  };

  struct remove_t {
    using interface_type = api_v1;
    static constexpr std::uint32_t proc = 2;
    static constexpr const char *proc_name = "remove";
    using arg_type = longstring;
    using arg_wire_type = longstring;
    using res_type = RPCBool;
    using res_wire_type = RPCBool;
    
    template<typename C, typename...A> static auto
    dispatch(C &&c, A &&...a) ->
    decltype(c.remove(std::forward<A>(a)...)) {
      return c.remove(std::forward<A>(a)...);
    }
    
    template<typename C, typename DropIfVoid, typename...A> static auto
    dispatch_dropvoid(C &&c, DropIfVoid &&d, A &&...a) ->
    decltype(c.remove(std::forward<DropIfVoid>(d), std::forward<A>(a)...)) {
      return c.remove(std::forward<DropIfVoid>(d), std::forward<A>(a)...);
    }
  };

  struct get_t {
    using interface_type = api_v1;
    static constexpr std::uint32_t proc = 3;
    static constexpr const char *proc_name = "get";
    using arg_type = longstring;
    using arg_wire_type = longstring;
    using res_type = RPCString;
    using res_wire_type = RPCString;
    
    template<typename C, typename...A> static auto
    dispatch(C &&c, A &&...a) ->
    decltype(c.get(std::forward<A>(a)...)) {
      return c.get(std::forward<A>(a)...);
    }
    
    template<typename C, typename DropIfVoid, typename...A> static auto
    dispatch_dropvoid(C &&c, DropIfVoid &&d, A &&...a) ->
    decltype(c.get(std::forward<DropIfVoid>(d), std::forward<A>(a)...)) {
      return c.get(std::forward<DropIfVoid>(d), std::forward<A>(a)...);
    }
  };

  struct set_t {
    using interface_type = api_v1;
    static constexpr std::uint32_t proc = 4;
    static constexpr const char *proc_name = "set";
    using arg_type = kvpair;
    using arg_wire_type = kvpair;
    using res_type = RPCBool;
    using res_wire_type = RPCBool;
    
    template<typename C, typename...A> static auto
    dispatch(C &&c, A &&...a) ->
    decltype(c.set(std::forward<A>(a)...)) {
      return c.set(std::forward<A>(a)...);
    }
    
    template<typename C, typename DropIfVoid, typename...A> static auto
    dispatch_dropvoid(C &&c, DropIfVoid &&d, A &&...a) ->
    decltype(c.set(std::forward<DropIfVoid>(d), std::forward<A>(a)...)) {
      return c.set(std::forward<DropIfVoid>(d), std::forward<A>(a)...);
    }
  };

  struct list_t {
    using interface_type = api_v1;
    static constexpr std::uint32_t proc = 5;
    static constexpr const char *proc_name = "list";
    using arg_type = longstring;
    using arg_wire_type = longstring;
    using res_type = RPCSet;
    using res_wire_type = RPCSet;
    
    template<typename C, typename...A> static auto
    dispatch(C &&c, A &&...a) ->
    decltype(c.list(std::forward<A>(a)...)) {
      return c.list(std::forward<A>(a)...);
    }
    
    template<typename C, typename DropIfVoid, typename...A> static auto
    dispatch_dropvoid(C &&c, DropIfVoid &&d, A &&...a) ->
    decltype(c.list(std::forward<DropIfVoid>(d), std::forward<A>(a)...)) {
      return c.list(std::forward<DropIfVoid>(d), std::forward<A>(a)...);
    }
  };

  template<typename T, typename...A> static bool
  call_dispatch(T &&t, std::uint32_t proc, A &&...a) {
    switch(proc) {
    case 1:
      t.template dispatch<create_t>(std::forward<A>(a)...);
      return true;
    case 2:
      t.template dispatch<remove_t>(std::forward<A>(a)...);
      return true;
    case 3:
      t.template dispatch<get_t>(std::forward<A>(a)...);
      return true;
    case 4:
      t.template dispatch<set_t>(std::forward<A>(a)...);
      return true;
    case 5:
      t.template dispatch<list_t>(std::forward<A>(a)...);
      return true;
    }
    return false;
  }

  template<typename _XDRBASE> struct client : _XDRBASE {
    using _XDRBASE::_XDRBASE;

    template<typename..._XDRARGS> auto
    create(_XDRARGS &&..._xdr_args) ->
    decltype(this->_XDRBASE::template invoke<create_t>(_xdr_args...)) {
      return this->_XDRBASE::template invoke<create_t>(_xdr_args...);
    }

    template<typename..._XDRARGS> auto
    remove(_XDRARGS &&..._xdr_args) ->
    decltype(this->_XDRBASE::template invoke<remove_t>(_xdr_args...)) {
      return this->_XDRBASE::template invoke<remove_t>(_xdr_args...);
    }

    template<typename..._XDRARGS> auto
    get(_XDRARGS &&..._xdr_args) ->
    decltype(this->_XDRBASE::template invoke<get_t>(_xdr_args...)) {
      return this->_XDRBASE::template invoke<get_t>(_xdr_args...);
    }

    template<typename..._XDRARGS> auto
    set(_XDRARGS &&..._xdr_args) ->
    decltype(this->_XDRBASE::template invoke<set_t>(_xdr_args...)) {
      return this->_XDRBASE::template invoke<set_t>(_xdr_args...);
    }

    template<typename..._XDRARGS> auto
    list(_XDRARGS &&..._xdr_args) ->
    decltype(this->_XDRBASE::template invoke<list_t>(_xdr_args...)) {
      return this->_XDRBASE::template invoke<list_t>(_xdr_args...);
    }
  };
};

#endif // !__XDR_INCLUDE_SERVER_HH_INCLUDED__
