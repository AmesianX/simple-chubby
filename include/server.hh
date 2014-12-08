// -*- C++ -*-
// Automatically generated from include/server.x.
// DO NOT EDIT or your changes may be overwritten

#ifndef __XDR_SERVER_HH_INCLUDED__
#define __XDR_SERVER_HH_INCLUDED__ 1

#include <xdrpp/types.h>

using longstring = xdr::xstring<>;
using ErrorCode = std::uint32_t;
using Mode = std::uint64_t;

struct MetaData {
  std::uint64_t instance_number{};
  std::uint64_t content_generation_number{};
  std::uint64_t lock_generation_number{};
  std::uint64_t file_content_checksum{};
  bool is_directory{};
};
namespace xdr {
template<> struct xdr_traits<::MetaData>
  : xdr_struct_base<field_ptr<::MetaData,
                              decltype(::MetaData::instance_number),
                              &::MetaData::instance_number>,
                    field_ptr<::MetaData,
                              decltype(::MetaData::content_generation_number),
                              &::MetaData::content_generation_number>,
                    field_ptr<::MetaData,
                              decltype(::MetaData::lock_generation_number),
                              &::MetaData::lock_generation_number>,
                    field_ptr<::MetaData,
                              decltype(::MetaData::file_content_checksum),
                              &::MetaData::file_content_checksum>,
                    field_ptr<::MetaData,
                              decltype(::MetaData::is_directory),
                              &::MetaData::is_directory>> {
  template<typename Archive> static void
  save(Archive &ar, const ::MetaData &obj) {
    archive(ar, obj.instance_number, "instance_number");
    archive(ar, obj.content_generation_number, "content_generation_number");
    archive(ar, obj.lock_generation_number, "lock_generation_number");
    archive(ar, obj.file_content_checksum, "file_content_checksum");
    archive(ar, obj.is_directory, "is_directory");
  }
  template<typename Archive> static void
  load(Archive &ar, ::MetaData &obj) {
    archive(ar, obj.instance_number, "instance_number");
    archive(ar, obj.content_generation_number, "content_generation_number");
    archive(ar, obj.lock_generation_number, "lock_generation_number");
    archive(ar, obj.file_content_checksum, "file_content_checksum");
    archive(ar, obj.is_directory, "is_directory");
  }
};
}

using FileContent = longstring;
using DirectoryContent = xdr::xvector<longstring>;

struct FileHandler {
  std::uint64_t magic_number{};
  std::uint64_t master_sequence_number{};
  longstring file_name{};
  std::uint64_t instance_number{};
  bool write_is_allowed{};
};
namespace xdr {
template<> struct xdr_traits<::FileHandler>
  : xdr_struct_base<field_ptr<::FileHandler,
                              decltype(::FileHandler::magic_number),
                              &::FileHandler::magic_number>,
                    field_ptr<::FileHandler,
                              decltype(::FileHandler::master_sequence_number),
                              &::FileHandler::master_sequence_number>,
                    field_ptr<::FileHandler,
                              decltype(::FileHandler::file_name),
                              &::FileHandler::file_name>,
                    field_ptr<::FileHandler,
                              decltype(::FileHandler::instance_number),
                              &::FileHandler::instance_number>,
                    field_ptr<::FileHandler,
                              decltype(::FileHandler::write_is_allowed),
                              &::FileHandler::write_is_allowed>> {
  template<typename Archive> static void
  save(Archive &ar, const ::FileHandler &obj) {
    archive(ar, obj.magic_number, "magic_number");
    archive(ar, obj.master_sequence_number, "master_sequence_number");
    archive(ar, obj.file_name, "file_name");
    archive(ar, obj.instance_number, "instance_number");
    archive(ar, obj.write_is_allowed, "write_is_allowed");
  }
  template<typename Archive> static void
  load(Archive &ar, ::FileHandler &obj) {
    archive(ar, obj.magic_number, "magic_number");
    archive(ar, obj.master_sequence_number, "master_sequence_number");
    archive(ar, obj.file_name, "file_name");
    archive(ar, obj.instance_number, "instance_number");
    archive(ar, obj.write_is_allowed, "write_is_allowed");
  }
};
}

struct ArgOpen {
  longstring name{};
  Mode mode{};
};
namespace xdr {
template<> struct xdr_traits<::ArgOpen>
  : xdr_struct_base<field_ptr<::ArgOpen,
                              decltype(::ArgOpen::name),
                              &::ArgOpen::name>,
                    field_ptr<::ArgOpen,
                              decltype(::ArgOpen::mode),
                              &::ArgOpen::mode>> {
  template<typename Archive> static void
  save(Archive &ar, const ::ArgOpen &obj) {
    archive(ar, obj.name, "name");
    archive(ar, obj.mode, "mode");
  }
  template<typename Archive> static void
  load(Archive &ar, ::ArgOpen &obj) {
    archive(ar, obj.name, "name");
    archive(ar, obj.mode, "mode");
  }
};
}

struct ArgReopen {
  FileHandler fd{};
  Mode mode{};
};
namespace xdr {
template<> struct xdr_traits<::ArgReopen>
  : xdr_struct_base<field_ptr<::ArgReopen,
                              decltype(::ArgReopen::fd),
                              &::ArgReopen::fd>,
                    field_ptr<::ArgReopen,
                              decltype(::ArgReopen::mode),
                              &::ArgReopen::mode>> {
  template<typename Archive> static void
  save(Archive &ar, const ::ArgReopen &obj) {
    archive(ar, obj.fd, "fd");
    archive(ar, obj.mode, "mode");
  }
  template<typename Archive> static void
  load(Archive &ar, ::ArgReopen &obj) {
    archive(ar, obj.fd, "fd");
    archive(ar, obj.mode, "mode");
  }
};
}

struct ArgSetContents {
  FileHandler fd{};
  FileContent content{};
};
namespace xdr {
template<> struct xdr_traits<::ArgSetContents>
  : xdr_struct_base<field_ptr<::ArgSetContents,
                              decltype(::ArgSetContents::fd),
                              &::ArgSetContents::fd>,
                    field_ptr<::ArgSetContents,
                              decltype(::ArgSetContents::content),
                              &::ArgSetContents::content>> {
  template<typename Archive> static void
  save(Archive &ar, const ::ArgSetContents &obj) {
    archive(ar, obj.fd, "fd");
    archive(ar, obj.content, "content");
  }
  template<typename Archive> static void
  load(Archive &ar, ::ArgSetContents &obj) {
    archive(ar, obj.fd, "fd");
    archive(ar, obj.content, "content");
  }
};
}

struct ContentsAndStat {
  FileContent content{};
  MetaData stat{};
};
namespace xdr {
template<> struct xdr_traits<::ContentsAndStat>
  : xdr_struct_base<field_ptr<::ContentsAndStat,
                              decltype(::ContentsAndStat::content),
                              &::ContentsAndStat::content>,
                    field_ptr<::ContentsAndStat,
                              decltype(::ContentsAndStat::stat),
                              &::ContentsAndStat::stat>> {
  template<typename Archive> static void
  save(Archive &ar, const ::ContentsAndStat &obj) {
    archive(ar, obj.content, "content");
    archive(ar, obj.stat, "stat");
  }
  template<typename Archive> static void
  load(Archive &ar, ::ContentsAndStat &obj) {
    archive(ar, obj.content, "content");
    archive(ar, obj.stat, "stat");
  }
};
}

struct RetBool {
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
      _f(&RetBool::val_, std::forward<A>(a)...);
      return true;
    case 1:
      _f(&RetBool::errCode_, std::forward<A>(a)...);
      return true;
    default:
      return true;
    }
  }

  std::uint32_t _xdr_discriminant() const { return discriminant_; }
  void _xdr_discriminant(std::uint32_t which, bool validate = true) {
    int fnum = _xdr_field_number(which);
    if (fnum < 0 && validate)
      throw xdr::xdr_bad_discriminant("bad value of discriminant in RetBool");
    if (fnum != _xdr_field_number(discriminant_)) {
      this->~RetBool();
      discriminant_ = which;
      _xdr_with_mem_ptr(xdr::field_constructor, discriminant_, *this);
    }
  }
  RetBool(std::uint32_t which = std::uint32_t{}) : discriminant_(which) {
    _xdr_with_mem_ptr(xdr::field_constructor, discriminant_, *this);
  }
  RetBool(const RetBool &source) : discriminant_(source.discriminant_) {
    _xdr_with_mem_ptr(xdr::field_constructor, discriminant_, *this, source);
  }
  RetBool(RetBool &&source) : discriminant_(source.discriminant_) {
    _xdr_with_mem_ptr(xdr::field_constructor, discriminant_, *this,
                      std::move(source));
  }
  ~RetBool() { _xdr_with_mem_ptr(xdr::field_destructor, discriminant_, *this); }
  RetBool &operator=(const RetBool &source) {
    if (_xdr_field_number(discriminant_) 
        == _xdr_field_number(source.discriminant_))
      _xdr_with_mem_ptr(xdr::field_assigner, discriminant_, *this, source);
    else {
      this->~RetBool();
      discriminant_ = std::uint32_t(-1);
      _xdr_with_mem_ptr(xdr::field_constructor, discriminant_, *this, source);
    }
    discriminant_ = source.discriminant_;
    return *this;
  }
  RetBool &operator=(RetBool &&source) {
    if (_xdr_field_number(discriminant_)
         == _xdr_field_number(source.discriminant_))
      _xdr_with_mem_ptr(xdr::field_assigner, discriminant_, *this,
                        std::move(source));
    else {
      this->~RetBool();
      discriminant_ = std::uint32_t(-1);
      _xdr_with_mem_ptr(xdr::field_constructor, discriminant_, *this,
                        std::move(source));
    }
    discriminant_ = source.discriminant_;
    return *this;
  }

  std::uint32_t discriminant() const { return std::uint32_t(discriminant_); }
  RetBool &discriminant(unsigned _xdr_d, bool _xdr_validate = true) {
    _xdr_discriminant(_xdr_d, _xdr_validate);
    return *this;
  }

  bool &val() {
    if (_xdr_field_number(discriminant_) == 1)
      return val_;
    throw xdr::xdr_wrong_union("RetBool: val accessed when not selected");
  }
  const bool &val() const {
    if (_xdr_field_number(discriminant_) == 1)
      return val_;
    throw xdr::xdr_wrong_union("RetBool: val accessed when not selected");
  }
  ErrorCode &errCode() {
    if (_xdr_field_number(discriminant_) == 2)
      return errCode_;
    throw xdr::xdr_wrong_union("RetBool: errCode accessed when not selected");
  }
  const ErrorCode &errCode() const {
    if (_xdr_field_number(discriminant_) == 2)
      return errCode_;
    throw xdr::xdr_wrong_union("RetBool: errCode accessed when not selected");
  }
};
namespace xdr {
template<> struct xdr_traits<::RetBool> : xdr_traits_base {
  static constexpr bool is_class = true;
  static constexpr bool is_union = true;
  static constexpr bool has_fixed_size = false;

  using union_type = ::RetBool;
  using discriminant_type = decltype(std::declval<union_type>().discriminant());

  static constexpr const char *union_field_name(std::uint32_t which) {
    return which == 0 ? "val"
      : which == 1 ? "errCode"
      : nullptr;
  }
  static const char *union_field_name(const union_type &u) {
    return union_field_name(u._xdr_discriminant());
  }

  static std::size_t serial_size(const ::RetBool &obj) {
    std::size_t size = 0;
    if (!obj._xdr_with_mem_ptr(field_size, obj._xdr_discriminant(), obj, size))
      throw xdr_bad_discriminant("bad value of discriminant in RetBool");
    return size + 4;
  }
  template<typename Archive> static void
  save(Archive &ar, const ::RetBool &obj) {
    xdr::archive(ar, obj.discriminant(), "discriminant");
    if (!obj._xdr_with_mem_ptr(field_archiver, obj.discriminant(), ar, obj,
                               union_field_name(obj)))
      throw xdr_bad_discriminant("bad value of discriminant in RetBool");
  }
  template<typename Archive> static void
  load(Archive &ar, ::RetBool &obj) {
    discriminant_type which;
    xdr::archive(ar, which, "discriminant");
    obj.discriminant(which);
    obj._xdr_with_mem_ptr(field_archiver, obj.discriminant(), ar, obj,
                          union_field_name(which));
  }
};
}

struct RetFd {
private:
  std::uint32_t discriminant_;
  union {
    FileHandler val_;
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
      _f(&RetFd::val_, std::forward<A>(a)...);
      return true;
    case 1:
      _f(&RetFd::errCode_, std::forward<A>(a)...);
      return true;
    default:
      return true;
    }
  }

  std::uint32_t _xdr_discriminant() const { return discriminant_; }
  void _xdr_discriminant(std::uint32_t which, bool validate = true) {
    int fnum = _xdr_field_number(which);
    if (fnum < 0 && validate)
      throw xdr::xdr_bad_discriminant("bad value of discriminant in RetFd");
    if (fnum != _xdr_field_number(discriminant_)) {
      this->~RetFd();
      discriminant_ = which;
      _xdr_with_mem_ptr(xdr::field_constructor, discriminant_, *this);
    }
  }
  RetFd(std::uint32_t which = std::uint32_t{}) : discriminant_(which) {
    _xdr_with_mem_ptr(xdr::field_constructor, discriminant_, *this);
  }
  RetFd(const RetFd &source) : discriminant_(source.discriminant_) {
    _xdr_with_mem_ptr(xdr::field_constructor, discriminant_, *this, source);
  }
  RetFd(RetFd &&source) : discriminant_(source.discriminant_) {
    _xdr_with_mem_ptr(xdr::field_constructor, discriminant_, *this,
                      std::move(source));
  }
  ~RetFd() { _xdr_with_mem_ptr(xdr::field_destructor, discriminant_, *this); }
  RetFd &operator=(const RetFd &source) {
    if (_xdr_field_number(discriminant_) 
        == _xdr_field_number(source.discriminant_))
      _xdr_with_mem_ptr(xdr::field_assigner, discriminant_, *this, source);
    else {
      this->~RetFd();
      discriminant_ = std::uint32_t(-1);
      _xdr_with_mem_ptr(xdr::field_constructor, discriminant_, *this, source);
    }
    discriminant_ = source.discriminant_;
    return *this;
  }
  RetFd &operator=(RetFd &&source) {
    if (_xdr_field_number(discriminant_)
         == _xdr_field_number(source.discriminant_))
      _xdr_with_mem_ptr(xdr::field_assigner, discriminant_, *this,
                        std::move(source));
    else {
      this->~RetFd();
      discriminant_ = std::uint32_t(-1);
      _xdr_with_mem_ptr(xdr::field_constructor, discriminant_, *this,
                        std::move(source));
    }
    discriminant_ = source.discriminant_;
    return *this;
  }

  std::uint32_t discriminant() const { return std::uint32_t(discriminant_); }
  RetFd &discriminant(unsigned _xdr_d, bool _xdr_validate = true) {
    _xdr_discriminant(_xdr_d, _xdr_validate);
    return *this;
  }

  FileHandler &val() {
    if (_xdr_field_number(discriminant_) == 1)
      return val_;
    throw xdr::xdr_wrong_union("RetFd: val accessed when not selected");
  }
  const FileHandler &val() const {
    if (_xdr_field_number(discriminant_) == 1)
      return val_;
    throw xdr::xdr_wrong_union("RetFd: val accessed when not selected");
  }
  ErrorCode &errCode() {
    if (_xdr_field_number(discriminant_) == 2)
      return errCode_;
    throw xdr::xdr_wrong_union("RetFd: errCode accessed when not selected");
  }
  const ErrorCode &errCode() const {
    if (_xdr_field_number(discriminant_) == 2)
      return errCode_;
    throw xdr::xdr_wrong_union("RetFd: errCode accessed when not selected");
  }
};
namespace xdr {
template<> struct xdr_traits<::RetFd> : xdr_traits_base {
  static constexpr bool is_class = true;
  static constexpr bool is_union = true;
  static constexpr bool has_fixed_size = false;

  using union_type = ::RetFd;
  using discriminant_type = decltype(std::declval<union_type>().discriminant());

  static constexpr const char *union_field_name(std::uint32_t which) {
    return which == 0 ? "val"
      : which == 1 ? "errCode"
      : nullptr;
  }
  static const char *union_field_name(const union_type &u) {
    return union_field_name(u._xdr_discriminant());
  }

  static std::size_t serial_size(const ::RetFd &obj) {
    std::size_t size = 0;
    if (!obj._xdr_with_mem_ptr(field_size, obj._xdr_discriminant(), obj, size))
      throw xdr_bad_discriminant("bad value of discriminant in RetFd");
    return size + 4;
  }
  template<typename Archive> static void
  save(Archive &ar, const ::RetFd &obj) {
    xdr::archive(ar, obj.discriminant(), "discriminant");
    if (!obj._xdr_with_mem_ptr(field_archiver, obj.discriminant(), ar, obj,
                               union_field_name(obj)))
      throw xdr_bad_discriminant("bad value of discriminant in RetFd");
  }
  template<typename Archive> static void
  load(Archive &ar, ::RetFd &obj) {
    discriminant_type which;
    xdr::archive(ar, which, "discriminant");
    obj.discriminant(which);
    obj._xdr_with_mem_ptr(field_archiver, obj.discriminant(), ar, obj,
                          union_field_name(which));
  }
};
}

struct RetContentsAndStat {
private:
  std::uint32_t discriminant_;
  union {
    ContentsAndStat val_;
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
      _f(&RetContentsAndStat::val_, std::forward<A>(a)...);
      return true;
    case 1:
      _f(&RetContentsAndStat::errCode_, std::forward<A>(a)...);
      return true;
    default:
      return true;
    }
  }

  std::uint32_t _xdr_discriminant() const { return discriminant_; }
  void _xdr_discriminant(std::uint32_t which, bool validate = true) {
    int fnum = _xdr_field_number(which);
    if (fnum < 0 && validate)
      throw xdr::xdr_bad_discriminant("bad value of discriminant in RetContentsAndStat");
    if (fnum != _xdr_field_number(discriminant_)) {
      this->~RetContentsAndStat();
      discriminant_ = which;
      _xdr_with_mem_ptr(xdr::field_constructor, discriminant_, *this);
    }
  }
  RetContentsAndStat(std::uint32_t which = std::uint32_t{}) : discriminant_(which) {
    _xdr_with_mem_ptr(xdr::field_constructor, discriminant_, *this);
  }
  RetContentsAndStat(const RetContentsAndStat &source) : discriminant_(source.discriminant_) {
    _xdr_with_mem_ptr(xdr::field_constructor, discriminant_, *this, source);
  }
  RetContentsAndStat(RetContentsAndStat &&source) : discriminant_(source.discriminant_) {
    _xdr_with_mem_ptr(xdr::field_constructor, discriminant_, *this,
                      std::move(source));
  }
  ~RetContentsAndStat() { _xdr_with_mem_ptr(xdr::field_destructor, discriminant_, *this); }
  RetContentsAndStat &operator=(const RetContentsAndStat &source) {
    if (_xdr_field_number(discriminant_) 
        == _xdr_field_number(source.discriminant_))
      _xdr_with_mem_ptr(xdr::field_assigner, discriminant_, *this, source);
    else {
      this->~RetContentsAndStat();
      discriminant_ = std::uint32_t(-1);
      _xdr_with_mem_ptr(xdr::field_constructor, discriminant_, *this, source);
    }
    discriminant_ = source.discriminant_;
    return *this;
  }
  RetContentsAndStat &operator=(RetContentsAndStat &&source) {
    if (_xdr_field_number(discriminant_)
         == _xdr_field_number(source.discriminant_))
      _xdr_with_mem_ptr(xdr::field_assigner, discriminant_, *this,
                        std::move(source));
    else {
      this->~RetContentsAndStat();
      discriminant_ = std::uint32_t(-1);
      _xdr_with_mem_ptr(xdr::field_constructor, discriminant_, *this,
                        std::move(source));
    }
    discriminant_ = source.discriminant_;
    return *this;
  }

  std::uint32_t discriminant() const { return std::uint32_t(discriminant_); }
  RetContentsAndStat &discriminant(unsigned _xdr_d, bool _xdr_validate = true) {
    _xdr_discriminant(_xdr_d, _xdr_validate);
    return *this;
  }

  ContentsAndStat &val() {
    if (_xdr_field_number(discriminant_) == 1)
      return val_;
    throw xdr::xdr_wrong_union("RetContentsAndStat: val accessed when not selected");
  }
  const ContentsAndStat &val() const {
    if (_xdr_field_number(discriminant_) == 1)
      return val_;
    throw xdr::xdr_wrong_union("RetContentsAndStat: val accessed when not selected");
  }
  ErrorCode &errCode() {
    if (_xdr_field_number(discriminant_) == 2)
      return errCode_;
    throw xdr::xdr_wrong_union("RetContentsAndStat: errCode accessed when not selected");
  }
  const ErrorCode &errCode() const {
    if (_xdr_field_number(discriminant_) == 2)
      return errCode_;
    throw xdr::xdr_wrong_union("RetContentsAndStat: errCode accessed when not selected");
  }
};
namespace xdr {
template<> struct xdr_traits<::RetContentsAndStat> : xdr_traits_base {
  static constexpr bool is_class = true;
  static constexpr bool is_union = true;
  static constexpr bool has_fixed_size = false;

  using union_type = ::RetContentsAndStat;
  using discriminant_type = decltype(std::declval<union_type>().discriminant());

  static constexpr const char *union_field_name(std::uint32_t which) {
    return which == 0 ? "val"
      : which == 1 ? "errCode"
      : nullptr;
  }
  static const char *union_field_name(const union_type &u) {
    return union_field_name(u._xdr_discriminant());
  }

  static std::size_t serial_size(const ::RetContentsAndStat &obj) {
    std::size_t size = 0;
    if (!obj._xdr_with_mem_ptr(field_size, obj._xdr_discriminant(), obj, size))
      throw xdr_bad_discriminant("bad value of discriminant in RetContentsAndStat");
    return size + 4;
  }
  template<typename Archive> static void
  save(Archive &ar, const ::RetContentsAndStat &obj) {
    xdr::archive(ar, obj.discriminant(), "discriminant");
    if (!obj._xdr_with_mem_ptr(field_archiver, obj.discriminant(), ar, obj,
                               union_field_name(obj)))
      throw xdr_bad_discriminant("bad value of discriminant in RetContentsAndStat");
  }
  template<typename Archive> static void
  load(Archive &ar, ::RetContentsAndStat &obj) {
    discriminant_type which;
    xdr::archive(ar, which, "discriminant");
    obj.discriminant(which);
    obj._xdr_with_mem_ptr(field_archiver, obj.discriminant(), ar, obj,
                          union_field_name(which));
  }
};
}

struct api_v1 {
  static constexpr std::uint32_t program = 1074036870;
  static constexpr const char *program_name = "server_api";
  static constexpr std::uint32_t version = 1;
  static constexpr const char *version_name = "api_v1";

  struct fileOpen_t {
    using interface_type = api_v1;
    static constexpr std::uint32_t proc = 1;
    static constexpr const char *proc_name = "fileOpen";
    using arg_type = ArgOpen;
    using arg_wire_type = ArgOpen;
    using res_type = RetFd;
    using res_wire_type = RetFd;
    
    template<typename C, typename...A> static auto
    dispatch(C &&c, A &&...a) ->
    decltype(c.fileOpen(std::forward<A>(a)...)) {
      return c.fileOpen(std::forward<A>(a)...);
    }
    
    template<typename C, typename DropIfVoid, typename...A> static auto
    dispatch_dropvoid(C &&c, DropIfVoid &&d, A &&...a) ->
    decltype(c.fileOpen(std::forward<DropIfVoid>(d), std::forward<A>(a)...)) {
      return c.fileOpen(std::forward<DropIfVoid>(d), std::forward<A>(a)...);
    }
  };

  struct fileClose_t {
    using interface_type = api_v1;
    static constexpr std::uint32_t proc = 2;
    static constexpr const char *proc_name = "fileClose";
    using arg_type = FileHandler;
    using arg_wire_type = FileHandler;
    using res_type = RetBool;
    using res_wire_type = RetBool;
    
    template<typename C, typename...A> static auto
    dispatch(C &&c, A &&...a) ->
    decltype(c.fileClose(std::forward<A>(a)...)) {
      return c.fileClose(std::forward<A>(a)...);
    }
    
    template<typename C, typename DropIfVoid, typename...A> static auto
    dispatch_dropvoid(C &&c, DropIfVoid &&d, A &&...a) ->
    decltype(c.fileClose(std::forward<DropIfVoid>(d), std::forward<A>(a)...)) {
      return c.fileClose(std::forward<DropIfVoid>(d), std::forward<A>(a)...);
    }
  };

  struct fileDelete_t {
    using interface_type = api_v1;
    static constexpr std::uint32_t proc = 3;
    static constexpr const char *proc_name = "fileDelete";
    using arg_type = FileHandler;
    using arg_wire_type = FileHandler;
    using res_type = RetBool;
    using res_wire_type = RetBool;
    
    template<typename C, typename...A> static auto
    dispatch(C &&c, A &&...a) ->
    decltype(c.fileDelete(std::forward<A>(a)...)) {
      return c.fileDelete(std::forward<A>(a)...);
    }
    
    template<typename C, typename DropIfVoid, typename...A> static auto
    dispatch_dropvoid(C &&c, DropIfVoid &&d, A &&...a) ->
    decltype(c.fileDelete(std::forward<DropIfVoid>(d), std::forward<A>(a)...)) {
      return c.fileDelete(std::forward<DropIfVoid>(d), std::forward<A>(a)...);
    }
  };

  struct getContentsAndStat_t {
    using interface_type = api_v1;
    static constexpr std::uint32_t proc = 4;
    static constexpr const char *proc_name = "getContentsAndStat";
    using arg_type = FileHandler;
    using arg_wire_type = FileHandler;
    using res_type = RetContentsAndStat;
    using res_wire_type = RetContentsAndStat;
    
    template<typename C, typename...A> static auto
    dispatch(C &&c, A &&...a) ->
    decltype(c.getContentsAndStat(std::forward<A>(a)...)) {
      return c.getContentsAndStat(std::forward<A>(a)...);
    }
    
    template<typename C, typename DropIfVoid, typename...A> static auto
    dispatch_dropvoid(C &&c, DropIfVoid &&d, A &&...a) ->
    decltype(c.getContentsAndStat(std::forward<DropIfVoid>(d), std::forward<A>(a)...)) {
      return c.getContentsAndStat(std::forward<DropIfVoid>(d), std::forward<A>(a)...);
    }
  };

  struct setContents_t {
    using interface_type = api_v1;
    static constexpr std::uint32_t proc = 5;
    static constexpr const char *proc_name = "setContents";
    using arg_type = ArgSetContents;
    using arg_wire_type = ArgSetContents;
    using res_type = RetBool;
    using res_wire_type = RetBool;
    
    template<typename C, typename...A> static auto
    dispatch(C &&c, A &&...a) ->
    decltype(c.setContents(std::forward<A>(a)...)) {
      return c.setContents(std::forward<A>(a)...);
    }
    
    template<typename C, typename DropIfVoid, typename...A> static auto
    dispatch_dropvoid(C &&c, DropIfVoid &&d, A &&...a) ->
    decltype(c.setContents(std::forward<DropIfVoid>(d), std::forward<A>(a)...)) {
      return c.setContents(std::forward<DropIfVoid>(d), std::forward<A>(a)...);
    }
  };

  struct acquire_t {
    using interface_type = api_v1;
    static constexpr std::uint32_t proc = 6;
    static constexpr const char *proc_name = "acquire";
    using arg_type = FileHandler;
    using arg_wire_type = FileHandler;
    using res_type = RetBool;
    using res_wire_type = RetBool;
    
    template<typename C, typename...A> static auto
    dispatch(C &&c, A &&...a) ->
    decltype(c.acquire(std::forward<A>(a)...)) {
      return c.acquire(std::forward<A>(a)...);
    }
    
    template<typename C, typename DropIfVoid, typename...A> static auto
    dispatch_dropvoid(C &&c, DropIfVoid &&d, A &&...a) ->
    decltype(c.acquire(std::forward<DropIfVoid>(d), std::forward<A>(a)...)) {
      return c.acquire(std::forward<DropIfVoid>(d), std::forward<A>(a)...);
    }
  };

  struct tryAcquire_t {
    using interface_type = api_v1;
    static constexpr std::uint32_t proc = 7;
    static constexpr const char *proc_name = "tryAcquire";
    using arg_type = FileHandler;
    using arg_wire_type = FileHandler;
    using res_type = RetBool;
    using res_wire_type = RetBool;
    
    template<typename C, typename...A> static auto
    dispatch(C &&c, A &&...a) ->
    decltype(c.tryAcquire(std::forward<A>(a)...)) {
      return c.tryAcquire(std::forward<A>(a)...);
    }
    
    template<typename C, typename DropIfVoid, typename...A> static auto
    dispatch_dropvoid(C &&c, DropIfVoid &&d, A &&...a) ->
    decltype(c.tryAcquire(std::forward<DropIfVoid>(d), std::forward<A>(a)...)) {
      return c.tryAcquire(std::forward<DropIfVoid>(d), std::forward<A>(a)...);
    }
  };

  struct release_t {
    using interface_type = api_v1;
    static constexpr std::uint32_t proc = 8;
    static constexpr const char *proc_name = "release";
    using arg_type = FileHandler;
    using arg_wire_type = FileHandler;
    using res_type = RetBool;
    using res_wire_type = RetBool;
    
    template<typename C, typename...A> static auto
    dispatch(C &&c, A &&...a) ->
    decltype(c.release(std::forward<A>(a)...)) {
      return c.release(std::forward<A>(a)...);
    }
    
    template<typename C, typename DropIfVoid, typename...A> static auto
    dispatch_dropvoid(C &&c, DropIfVoid &&d, A &&...a) ->
    decltype(c.release(std::forward<DropIfVoid>(d), std::forward<A>(a)...)) {
      return c.release(std::forward<DropIfVoid>(d), std::forward<A>(a)...);
    }
  };

  struct startSession_t {
    using interface_type = api_v1;
    static constexpr std::uint32_t proc = 21;
    static constexpr const char *proc_name = "startSession";
    using arg_type = longstring;
    using arg_wire_type = longstring;
    using res_type = RetBool;
    using res_wire_type = RetBool;
    
    template<typename C, typename...A> static auto
    dispatch(C &&c, A &&...a) ->
    decltype(c.startSession(std::forward<A>(a)...)) {
      return c.startSession(std::forward<A>(a)...);
    }
    
    template<typename C, typename DropIfVoid, typename...A> static auto
    dispatch_dropvoid(C &&c, DropIfVoid &&d, A &&...a) ->
    decltype(c.startSession(std::forward<DropIfVoid>(d), std::forward<A>(a)...)) {
      return c.startSession(std::forward<DropIfVoid>(d), std::forward<A>(a)...);
    }
  };

  struct fileReopen_t {
    using interface_type = api_v1;
    static constexpr std::uint32_t proc = 22;
    static constexpr const char *proc_name = "fileReopen";
    using arg_type = ArgReopen;
    using arg_wire_type = ArgReopen;
    using res_type = RetBool;
    using res_wire_type = RetBool;
    
    template<typename C, typename...A> static auto
    dispatch(C &&c, A &&...a) ->
    decltype(c.fileReopen(std::forward<A>(a)...)) {
      return c.fileReopen(std::forward<A>(a)...);
    }
    
    template<typename C, typename DropIfVoid, typename...A> static auto
    dispatch_dropvoid(C &&c, DropIfVoid &&d, A &&...a) ->
    decltype(c.fileReopen(std::forward<DropIfVoid>(d), std::forward<A>(a)...)) {
      return c.fileReopen(std::forward<DropIfVoid>(d), std::forward<A>(a)...);
    }
  };

  struct increment_t {
    using interface_type = api_v1;
    static constexpr std::uint32_t proc = 991;
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
    using interface_type = api_v1;
    static constexpr std::uint32_t proc = 992;
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
    case 1:
      t.template dispatch<fileOpen_t>(std::forward<A>(a)...);
      return true;
    case 2:
      t.template dispatch<fileClose_t>(std::forward<A>(a)...);
      return true;
    case 3:
      t.template dispatch<fileDelete_t>(std::forward<A>(a)...);
      return true;
    case 4:
      t.template dispatch<getContentsAndStat_t>(std::forward<A>(a)...);
      return true;
    case 5:
      t.template dispatch<setContents_t>(std::forward<A>(a)...);
      return true;
    case 6:
      t.template dispatch<acquire_t>(std::forward<A>(a)...);
      return true;
    case 7:
      t.template dispatch<tryAcquire_t>(std::forward<A>(a)...);
      return true;
    case 8:
      t.template dispatch<release_t>(std::forward<A>(a)...);
      return true;
    case 21:
      t.template dispatch<startSession_t>(std::forward<A>(a)...);
      return true;
    case 22:
      t.template dispatch<fileReopen_t>(std::forward<A>(a)...);
      return true;
    case 991:
      t.template dispatch<increment_t>(std::forward<A>(a)...);
      return true;
    case 992:
      t.template dispatch<decrement_t>(std::forward<A>(a)...);
      return true;
    }
    return false;
  }

  template<typename _XDRBASE> struct client : _XDRBASE {
    using _XDRBASE::_XDRBASE;

    template<typename..._XDRARGS> auto
    fileOpen(_XDRARGS &&..._xdr_args) ->
    decltype(this->_XDRBASE::template invoke<fileOpen_t>(_xdr_args...)) {
      return this->_XDRBASE::template invoke<fileOpen_t>(_xdr_args...);
    }

    template<typename..._XDRARGS> auto
    fileClose(_XDRARGS &&..._xdr_args) ->
    decltype(this->_XDRBASE::template invoke<fileClose_t>(_xdr_args...)) {
      return this->_XDRBASE::template invoke<fileClose_t>(_xdr_args...);
    }

    template<typename..._XDRARGS> auto
    fileDelete(_XDRARGS &&..._xdr_args) ->
    decltype(this->_XDRBASE::template invoke<fileDelete_t>(_xdr_args...)) {
      return this->_XDRBASE::template invoke<fileDelete_t>(_xdr_args...);
    }

    template<typename..._XDRARGS> auto
    getContentsAndStat(_XDRARGS &&..._xdr_args) ->
    decltype(this->_XDRBASE::template invoke<getContentsAndStat_t>(_xdr_args...)) {
      return this->_XDRBASE::template invoke<getContentsAndStat_t>(_xdr_args...);
    }

    template<typename..._XDRARGS> auto
    setContents(_XDRARGS &&..._xdr_args) ->
    decltype(this->_XDRBASE::template invoke<setContents_t>(_xdr_args...)) {
      return this->_XDRBASE::template invoke<setContents_t>(_xdr_args...);
    }

    template<typename..._XDRARGS> auto
    acquire(_XDRARGS &&..._xdr_args) ->
    decltype(this->_XDRBASE::template invoke<acquire_t>(_xdr_args...)) {
      return this->_XDRBASE::template invoke<acquire_t>(_xdr_args...);
    }

    template<typename..._XDRARGS> auto
    tryAcquire(_XDRARGS &&..._xdr_args) ->
    decltype(this->_XDRBASE::template invoke<tryAcquire_t>(_xdr_args...)) {
      return this->_XDRBASE::template invoke<tryAcquire_t>(_xdr_args...);
    }

    template<typename..._XDRARGS> auto
    release(_XDRARGS &&..._xdr_args) ->
    decltype(this->_XDRBASE::template invoke<release_t>(_xdr_args...)) {
      return this->_XDRBASE::template invoke<release_t>(_xdr_args...);
    }

    template<typename..._XDRARGS> auto
    startSession(_XDRARGS &&..._xdr_args) ->
    decltype(this->_XDRBASE::template invoke<startSession_t>(_xdr_args...)) {
      return this->_XDRBASE::template invoke<startSession_t>(_xdr_args...);
    }

    template<typename..._XDRARGS> auto
    fileReopen(_XDRARGS &&..._xdr_args) ->
    decltype(this->_XDRBASE::template invoke<fileReopen_t>(_xdr_args...)) {
      return this->_XDRBASE::template invoke<fileReopen_t>(_xdr_args...);
    }

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

#endif // !__XDR_SERVER_HH_INCLUDED__
