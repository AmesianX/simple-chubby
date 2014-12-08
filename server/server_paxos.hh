// -*- C++ -*-
// Automatically generated from server_paxos.x.
// DO NOT EDIT or your changes may be overwritten

#ifndef __XDR_SERVER_PAXOS_HH_INCLUDED__
#define __XDR_SERVER_PAXOS_HH_INCLUDED__ 1

#include <xdrpp/types.h>

enum CallType : std::uint32_t {
  CHECK_AND_CREATE,
  CHECK_AND_OPEN,
  CHECK_AND_DELETE,
  CHECK_AND_READ,
  CHECK_AND_UPDATE,
  TEST_AND_SET_LOCK_OWNER,
  RESET_LOCK_OWNER,
  GET_STATES,
};
namespace xdr {
template<> struct xdr_traits<::CallType>
  : xdr_integral_base<::CallType, std::uint32_t> {
  static constexpr bool is_enum = true;
  static constexpr bool is_numeric = false;
  static const char *enum_name(::CallType val) {
    switch (val) {
    case ::CHECK_AND_CREATE:
      return "CHECK_AND_CREATE";
    case ::CHECK_AND_OPEN:
      return "CHECK_AND_OPEN";
    case ::CHECK_AND_DELETE:
      return "CHECK_AND_DELETE";
    case ::CHECK_AND_READ:
      return "CHECK_AND_READ";
    case ::CHECK_AND_UPDATE:
      return "CHECK_AND_UPDATE";
    case ::TEST_AND_SET_LOCK_OWNER:
      return "TEST_AND_SET_LOCK_OWNER";
    case ::RESET_LOCK_OWNER:
      return "RESET_LOCK_OWNER";
    case ::GET_STATES:
      return "GET_STATES";
    default:
      return nullptr;
    }
  }
};
}

struct NewMetaData {
  std::uint64_t instance_number{};
  std::uint64_t content_generation_number{};
  std::uint64_t lock_generation_number{};
  std::uint64_t file_content_checksum{};
  bool is_directory{};
};
namespace xdr {
template<> struct xdr_traits<::NewMetaData>
  : xdr_struct_base<field_ptr<::NewMetaData,
                              decltype(::NewMetaData::instance_number),
                              &::NewMetaData::instance_number>,
                    field_ptr<::NewMetaData,
                              decltype(::NewMetaData::content_generation_number),
                              &::NewMetaData::content_generation_number>,
                    field_ptr<::NewMetaData,
                              decltype(::NewMetaData::lock_generation_number),
                              &::NewMetaData::lock_generation_number>,
                    field_ptr<::NewMetaData,
                              decltype(::NewMetaData::file_content_checksum),
                              &::NewMetaData::file_content_checksum>,
                    field_ptr<::NewMetaData,
                              decltype(::NewMetaData::is_directory),
                              &::NewMetaData::is_directory>> {
  template<typename Archive> static void
  save(Archive &ar, const ::NewMetaData &obj) {
    archive(ar, obj.instance_number, "instance_number");
    archive(ar, obj.content_generation_number, "content_generation_number");
    archive(ar, obj.lock_generation_number, "lock_generation_number");
    archive(ar, obj.file_content_checksum, "file_content_checksum");
    archive(ar, obj.is_directory, "is_directory");
  }
  template<typename Archive> static void
  load(Archive &ar, ::NewMetaData &obj) {
    archive(ar, obj.instance_number, "instance_number");
    archive(ar, obj.content_generation_number, "content_generation_number");
    archive(ar, obj.lock_generation_number, "lock_generation_number");
    archive(ar, obj.file_content_checksum, "file_content_checksum");
    archive(ar, obj.is_directory, "is_directory");
  }
};
}

struct CheckAndCreateParam {
  xdr::xstring<> file_name{};
  bool is_dir{};
};
namespace xdr {
template<> struct xdr_traits<::CheckAndCreateParam>
  : xdr_struct_base<field_ptr<::CheckAndCreateParam,
                              decltype(::CheckAndCreateParam::file_name),
                              &::CheckAndCreateParam::file_name>,
                    field_ptr<::CheckAndCreateParam,
                              decltype(::CheckAndCreateParam::is_dir),
                              &::CheckAndCreateParam::is_dir>> {
  template<typename Archive> static void
  save(Archive &ar, const ::CheckAndCreateParam &obj) {
    archive(ar, obj.file_name, "file_name");
    archive(ar, obj.is_dir, "is_dir");
  }
  template<typename Archive> static void
  load(Archive &ar, ::CheckAndCreateParam &obj) {
    archive(ar, obj.file_name, "file_name");
    archive(ar, obj.is_dir, "is_dir");
  }
};
}

struct CheckAndCreateResult {
  std::uint32_t instance_number{};
  bool success{};
};
namespace xdr {
template<> struct xdr_traits<::CheckAndCreateResult>
  : xdr_struct_base<field_ptr<::CheckAndCreateResult,
                              decltype(::CheckAndCreateResult::instance_number),
                              &::CheckAndCreateResult::instance_number>,
                    field_ptr<::CheckAndCreateResult,
                              decltype(::CheckAndCreateResult::success),
                              &::CheckAndCreateResult::success>> {
  template<typename Archive> static void
  save(Archive &ar, const ::CheckAndCreateResult &obj) {
    archive(ar, obj.instance_number, "instance_number");
    archive(ar, obj.success, "success");
  }
  template<typename Archive> static void
  load(Archive &ar, ::CheckAndCreateResult &obj) {
    archive(ar, obj.instance_number, "instance_number");
    archive(ar, obj.success, "success");
  }
};
}

struct CheckAndOpenParam {
  xdr::xstring<> file_name{};
};
namespace xdr {
template<> struct xdr_traits<::CheckAndOpenParam>
  : xdr_struct_base<field_ptr<::CheckAndOpenParam,
                              decltype(::CheckAndOpenParam::file_name),
                              &::CheckAndOpenParam::file_name>> {
  template<typename Archive> static void
  save(Archive &ar, const ::CheckAndOpenParam &obj) {
    archive(ar, obj.file_name, "file_name");
  }
  template<typename Archive> static void
  load(Archive &ar, ::CheckAndOpenParam &obj) {
    archive(ar, obj.file_name, "file_name");
  }
};
}

struct CheckAndOpenResult {
  std::uint32_t instance_number{};
  bool success{};
};
namespace xdr {
template<> struct xdr_traits<::CheckAndOpenResult>
  : xdr_struct_base<field_ptr<::CheckAndOpenResult,
                              decltype(::CheckAndOpenResult::instance_number),
                              &::CheckAndOpenResult::instance_number>,
                    field_ptr<::CheckAndOpenResult,
                              decltype(::CheckAndOpenResult::success),
                              &::CheckAndOpenResult::success>> {
  template<typename Archive> static void
  save(Archive &ar, const ::CheckAndOpenResult &obj) {
    archive(ar, obj.instance_number, "instance_number");
    archive(ar, obj.success, "success");
  }
  template<typename Archive> static void
  load(Archive &ar, ::CheckAndOpenResult &obj) {
    archive(ar, obj.instance_number, "instance_number");
    archive(ar, obj.success, "success");
  }
};
}

struct CheckAndDeleteParam {
  xdr::xstring<> file_name{};
  std::uint32_t instance_number{};
};
namespace xdr {
template<> struct xdr_traits<::CheckAndDeleteParam>
  : xdr_struct_base<field_ptr<::CheckAndDeleteParam,
                              decltype(::CheckAndDeleteParam::file_name),
                              &::CheckAndDeleteParam::file_name>,
                    field_ptr<::CheckAndDeleteParam,
                              decltype(::CheckAndDeleteParam::instance_number),
                              &::CheckAndDeleteParam::instance_number>> {
  template<typename Archive> static void
  save(Archive &ar, const ::CheckAndDeleteParam &obj) {
    archive(ar, obj.file_name, "file_name");
    archive(ar, obj.instance_number, "instance_number");
  }
  template<typename Archive> static void
  load(Archive &ar, ::CheckAndDeleteParam &obj) {
    archive(ar, obj.file_name, "file_name");
    archive(ar, obj.instance_number, "instance_number");
  }
};
}

struct CheckAndDeleteResult {
  bool success{};
};
namespace xdr {
template<> struct xdr_traits<::CheckAndDeleteResult>
  : xdr_struct_base<field_ptr<::CheckAndDeleteResult,
                              decltype(::CheckAndDeleteResult::success),
                              &::CheckAndDeleteResult::success>> {
  template<typename Archive> static void
  save(Archive &ar, const ::CheckAndDeleteResult &obj) {
    archive(ar, obj.success, "success");
  }
  template<typename Archive> static void
  load(Archive &ar, ::CheckAndDeleteResult &obj) {
    archive(ar, obj.success, "success");
  }
};
}

struct CheckAndReadParam {
  xdr::xstring<> file_name{};
  std::uint32_t instance_number{};
};
namespace xdr {
template<> struct xdr_traits<::CheckAndReadParam>
  : xdr_struct_base<field_ptr<::CheckAndReadParam,
                              decltype(::CheckAndReadParam::file_name),
                              &::CheckAndReadParam::file_name>,
                    field_ptr<::CheckAndReadParam,
                              decltype(::CheckAndReadParam::instance_number),
                              &::CheckAndReadParam::instance_number>> {
  template<typename Archive> static void
  save(Archive &ar, const ::CheckAndReadParam &obj) {
    archive(ar, obj.file_name, "file_name");
    archive(ar, obj.instance_number, "instance_number");
  }
  template<typename Archive> static void
  load(Archive &ar, ::CheckAndReadParam &obj) {
    archive(ar, obj.file_name, "file_name");
    archive(ar, obj.instance_number, "instance_number");
  }
};
}

struct CheckAndReadResult {
  xdr::xstring<> content{};
  NewMetaData meta{};
  bool success{};
};
namespace xdr {
template<> struct xdr_traits<::CheckAndReadResult>
  : xdr_struct_base<field_ptr<::CheckAndReadResult,
                              decltype(::CheckAndReadResult::content),
                              &::CheckAndReadResult::content>,
                    field_ptr<::CheckAndReadResult,
                              decltype(::CheckAndReadResult::meta),
                              &::CheckAndReadResult::meta>,
                    field_ptr<::CheckAndReadResult,
                              decltype(::CheckAndReadResult::success),
                              &::CheckAndReadResult::success>> {
  template<typename Archive> static void
  save(Archive &ar, const ::CheckAndReadResult &obj) {
    archive(ar, obj.content, "content");
    archive(ar, obj.meta, "meta");
    archive(ar, obj.success, "success");
  }
  template<typename Archive> static void
  load(Archive &ar, ::CheckAndReadResult &obj) {
    archive(ar, obj.content, "content");
    archive(ar, obj.meta, "meta");
    archive(ar, obj.success, "success");
  }
};
}

struct CheckAndUpdateParam {
  xdr::xstring<> file_name{};
  std::uint32_t instance_number{};
  xdr::xstring<> content{};
};
namespace xdr {
template<> struct xdr_traits<::CheckAndUpdateParam>
  : xdr_struct_base<field_ptr<::CheckAndUpdateParam,
                              decltype(::CheckAndUpdateParam::file_name),
                              &::CheckAndUpdateParam::file_name>,
                    field_ptr<::CheckAndUpdateParam,
                              decltype(::CheckAndUpdateParam::instance_number),
                              &::CheckAndUpdateParam::instance_number>,
                    field_ptr<::CheckAndUpdateParam,
                              decltype(::CheckAndUpdateParam::content),
                              &::CheckAndUpdateParam::content>> {
  template<typename Archive> static void
  save(Archive &ar, const ::CheckAndUpdateParam &obj) {
    archive(ar, obj.file_name, "file_name");
    archive(ar, obj.instance_number, "instance_number");
    archive(ar, obj.content, "content");
  }
  template<typename Archive> static void
  load(Archive &ar, ::CheckAndUpdateParam &obj) {
    archive(ar, obj.file_name, "file_name");
    archive(ar, obj.instance_number, "instance_number");
    archive(ar, obj.content, "content");
  }
};
}

struct CheckAndUpdateResult {
  bool success{};
};
namespace xdr {
template<> struct xdr_traits<::CheckAndUpdateResult>
  : xdr_struct_base<field_ptr<::CheckAndUpdateResult,
                              decltype(::CheckAndUpdateResult::success),
                              &::CheckAndUpdateResult::success>> {
  template<typename Archive> static void
  save(Archive &ar, const ::CheckAndUpdateResult &obj) {
    archive(ar, obj.success, "success");
  }
  template<typename Archive> static void
  load(Archive &ar, ::CheckAndUpdateResult &obj) {
    archive(ar, obj.success, "success");
  }
};
}

struct TestAndSetLockOwnerParam {
  xdr::xstring<> file_name{};
  std::uint32_t instance_number{};
  xdr::xstring<> client_id{};
};
namespace xdr {
template<> struct xdr_traits<::TestAndSetLockOwnerParam>
  : xdr_struct_base<field_ptr<::TestAndSetLockOwnerParam,
                              decltype(::TestAndSetLockOwnerParam::file_name),
                              &::TestAndSetLockOwnerParam::file_name>,
                    field_ptr<::TestAndSetLockOwnerParam,
                              decltype(::TestAndSetLockOwnerParam::instance_number),
                              &::TestAndSetLockOwnerParam::instance_number>,
                    field_ptr<::TestAndSetLockOwnerParam,
                              decltype(::TestAndSetLockOwnerParam::client_id),
                              &::TestAndSetLockOwnerParam::client_id>> {
  template<typename Archive> static void
  save(Archive &ar, const ::TestAndSetLockOwnerParam &obj) {
    archive(ar, obj.file_name, "file_name");
    archive(ar, obj.instance_number, "instance_number");
    archive(ar, obj.client_id, "client_id");
  }
  template<typename Archive> static void
  load(Archive &ar, ::TestAndSetLockOwnerParam &obj) {
    archive(ar, obj.file_name, "file_name");
    archive(ar, obj.instance_number, "instance_number");
    archive(ar, obj.client_id, "client_id");
  }
};
}

struct TestAndSetLockOwnerResult {
  bool success{};
};
namespace xdr {
template<> struct xdr_traits<::TestAndSetLockOwnerResult>
  : xdr_struct_base<field_ptr<::TestAndSetLockOwnerResult,
                              decltype(::TestAndSetLockOwnerResult::success),
                              &::TestAndSetLockOwnerResult::success>> {
  template<typename Archive> static void
  save(Archive &ar, const ::TestAndSetLockOwnerResult &obj) {
    archive(ar, obj.success, "success");
  }
  template<typename Archive> static void
  load(Archive &ar, ::TestAndSetLockOwnerResult &obj) {
    archive(ar, obj.success, "success");
  }
};
}

struct ResetLockOwnerParam {
  xdr::xstring<> file_name{};
  std::uint32_t instance_number{};
};
namespace xdr {
template<> struct xdr_traits<::ResetLockOwnerParam>
  : xdr_struct_base<field_ptr<::ResetLockOwnerParam,
                              decltype(::ResetLockOwnerParam::file_name),
                              &::ResetLockOwnerParam::file_name>,
                    field_ptr<::ResetLockOwnerParam,
                              decltype(::ResetLockOwnerParam::instance_number),
                              &::ResetLockOwnerParam::instance_number>> {
  template<typename Archive> static void
  save(Archive &ar, const ::ResetLockOwnerParam &obj) {
    archive(ar, obj.file_name, "file_name");
    archive(ar, obj.instance_number, "instance_number");
  }
  template<typename Archive> static void
  load(Archive &ar, ::ResetLockOwnerParam &obj) {
    archive(ar, obj.file_name, "file_name");
    archive(ar, obj.instance_number, "instance_number");
  }
};
}

struct ResetLockOwnerResult {
  bool success{};
};
namespace xdr {
template<> struct xdr_traits<::ResetLockOwnerResult>
  : xdr_struct_base<field_ptr<::ResetLockOwnerResult,
                              decltype(::ResetLockOwnerResult::success),
                              &::ResetLockOwnerResult::success>> {
  template<typename Archive> static void
  save(Archive &ar, const ::ResetLockOwnerResult &obj) {
    archive(ar, obj.success, "success");
  }
  template<typename Archive> static void
  load(Archive &ar, ::ResetLockOwnerResult &obj) {
    archive(ar, obj.success, "success");
  }
};
}

struct GetStatesParam {
  bool dumb{};
};
namespace xdr {
template<> struct xdr_traits<::GetStatesParam>
  : xdr_struct_base<field_ptr<::GetStatesParam,
                              decltype(::GetStatesParam::dumb),
                              &::GetStatesParam::dumb>> {
  template<typename Archive> static void
  save(Archive &ar, const ::GetStatesParam &obj) {
    archive(ar, obj.dumb, "dumb");
  }
  template<typename Archive> static void
  load(Archive &ar, ::GetStatesParam &obj) {
    archive(ar, obj.dumb, "dumb");
  }
};
}

struct key_value {
  xdr::xstring<> key{};
  xdr::xstring<> value{};
};
namespace xdr {
template<> struct xdr_traits<::key_value>
  : xdr_struct_base<field_ptr<::key_value,
                              decltype(::key_value::key),
                              &::key_value::key>,
                    field_ptr<::key_value,
                              decltype(::key_value::value),
                              &::key_value::value>> {
  template<typename Archive> static void
  save(Archive &ar, const ::key_value &obj) {
    archive(ar, obj.key, "key");
    archive(ar, obj.value, "value");
  }
  template<typename Archive> static void
  load(Archive &ar, ::key_value &obj) {
    archive(ar, obj.key, "key");
    archive(ar, obj.value, "value");
  }
};
}

struct GetStatesResult {
  xdr::xvector<key_value> list{};
};
namespace xdr {
template<> struct xdr_traits<::GetStatesResult>
  : xdr_struct_base<field_ptr<::GetStatesResult,
                              decltype(::GetStatesResult::list),
                              &::GetStatesResult::list>> {
  template<typename Archive> static void
  save(Archive &ar, const ::GetStatesResult &obj) {
    archive(ar, obj.list, "list");
  }
  template<typename Archive> static void
  load(Archive &ar, ::GetStatesResult &obj) {
    archive(ar, obj.list, "list");
  }
};
}

struct ServerPaxosParam {
private:
  std::uint32_t type_;
  union {
    CheckAndCreateParam check_and_create_param_;
    CheckAndOpenParam check_and_open_param_;
    CheckAndDeleteParam check_and_delete_param_;
    CheckAndReadParam check_and_read_param_;
    CheckAndUpdateParam check_and_update_param_;
    TestAndSetLockOwnerParam check_and_test_lock_owner_param_;
    ResetLockOwnerParam reset_lock_owner_param_;
    GetStatesParam get_states_param_;
  };

public:
  static_assert (sizeof (unsigned) <= 4, "union discriminant must be 4 bytes");

  static constexpr int _xdr_field_number(std::uint32_t which) {
    return which == CHECK_AND_CREATE ? 1
      : which == CHECK_AND_OPEN ? 2
      : which == CHECK_AND_DELETE ? 3
      : which == CHECK_AND_READ ? 4
      : which == CHECK_AND_UPDATE ? 5
      : which == TEST_AND_SET_LOCK_OWNER ? 6
      : which == RESET_LOCK_OWNER ? 7
      : which == GET_STATES ? 8
      : -1;
  }
  template<typename _F, typename...A> static bool
  _xdr_with_mem_ptr(_F &_f, std::uint32_t which, A&&...a) {
    switch (which) {
    case CHECK_AND_CREATE:
      _f(&ServerPaxosParam::check_and_create_param_, std::forward<A>(a)...);
      return true;
    case CHECK_AND_OPEN:
      _f(&ServerPaxosParam::check_and_open_param_, std::forward<A>(a)...);
      return true;
    case CHECK_AND_DELETE:
      _f(&ServerPaxosParam::check_and_delete_param_, std::forward<A>(a)...);
      return true;
    case CHECK_AND_READ:
      _f(&ServerPaxosParam::check_and_read_param_, std::forward<A>(a)...);
      return true;
    case CHECK_AND_UPDATE:
      _f(&ServerPaxosParam::check_and_update_param_, std::forward<A>(a)...);
      return true;
    case TEST_AND_SET_LOCK_OWNER:
      _f(&ServerPaxosParam::check_and_test_lock_owner_param_, std::forward<A>(a)...);
      return true;
    case RESET_LOCK_OWNER:
      _f(&ServerPaxosParam::reset_lock_owner_param_, std::forward<A>(a)...);
      return true;
    case GET_STATES:
      _f(&ServerPaxosParam::get_states_param_, std::forward<A>(a)...);
      return true;
    }
    return false;
  }

  std::uint32_t _xdr_discriminant() const { return type_; }
  void _xdr_discriminant(std::uint32_t which, bool validate = true) {
    int fnum = _xdr_field_number(which);
    if (fnum < 0 && validate)
      throw xdr::xdr_bad_discriminant("bad value of type in ServerPaxosParam");
    if (fnum != _xdr_field_number(type_)) {
      this->~ServerPaxosParam();
      type_ = which;
      _xdr_with_mem_ptr(xdr::field_constructor, type_, *this);
    }
  }
  ServerPaxosParam(std::uint32_t which = std::uint32_t{}) : type_(which) {
    _xdr_with_mem_ptr(xdr::field_constructor, type_, *this);
  }
  ServerPaxosParam(const ServerPaxosParam &source) : type_(source.type_) {
    _xdr_with_mem_ptr(xdr::field_constructor, type_, *this, source);
  }
  ServerPaxosParam(ServerPaxosParam &&source) : type_(source.type_) {
    _xdr_with_mem_ptr(xdr::field_constructor, type_, *this,
                      std::move(source));
  }
  ~ServerPaxosParam() { _xdr_with_mem_ptr(xdr::field_destructor, type_, *this); }
  ServerPaxosParam &operator=(const ServerPaxosParam &source) {
    if (_xdr_field_number(type_) 
        == _xdr_field_number(source.type_))
      _xdr_with_mem_ptr(xdr::field_assigner, type_, *this, source);
    else {
      this->~ServerPaxosParam();
      type_ = std::uint32_t(-1);
      _xdr_with_mem_ptr(xdr::field_constructor, type_, *this, source);
    }
    type_ = source.type_;
    return *this;
  }
  ServerPaxosParam &operator=(ServerPaxosParam &&source) {
    if (_xdr_field_number(type_)
         == _xdr_field_number(source.type_))
      _xdr_with_mem_ptr(xdr::field_assigner, type_, *this,
                        std::move(source));
    else {
      this->~ServerPaxosParam();
      type_ = std::uint32_t(-1);
      _xdr_with_mem_ptr(xdr::field_constructor, type_, *this,
                        std::move(source));
    }
    type_ = source.type_;
    return *this;
  }

  std::uint32_t type() const { return std::uint32_t(type_); }
  ServerPaxosParam &type(unsigned _xdr_d, bool _xdr_validate = true) {
    _xdr_discriminant(_xdr_d, _xdr_validate);
    return *this;
  }

  CheckAndCreateParam &check_and_create_param() {
    if (_xdr_field_number(type_) == 1)
      return check_and_create_param_;
    throw xdr::xdr_wrong_union("ServerPaxosParam: check_and_create_param accessed when not selected");
  }
  const CheckAndCreateParam &check_and_create_param() const {
    if (_xdr_field_number(type_) == 1)
      return check_and_create_param_;
    throw xdr::xdr_wrong_union("ServerPaxosParam: check_and_create_param accessed when not selected");
  }
  CheckAndOpenParam &check_and_open_param() {
    if (_xdr_field_number(type_) == 2)
      return check_and_open_param_;
    throw xdr::xdr_wrong_union("ServerPaxosParam: check_and_open_param accessed when not selected");
  }
  const CheckAndOpenParam &check_and_open_param() const {
    if (_xdr_field_number(type_) == 2)
      return check_and_open_param_;
    throw xdr::xdr_wrong_union("ServerPaxosParam: check_and_open_param accessed when not selected");
  }
  CheckAndDeleteParam &check_and_delete_param() {
    if (_xdr_field_number(type_) == 3)
      return check_and_delete_param_;
    throw xdr::xdr_wrong_union("ServerPaxosParam: check_and_delete_param accessed when not selected");
  }
  const CheckAndDeleteParam &check_and_delete_param() const {
    if (_xdr_field_number(type_) == 3)
      return check_and_delete_param_;
    throw xdr::xdr_wrong_union("ServerPaxosParam: check_and_delete_param accessed when not selected");
  }
  CheckAndReadParam &check_and_read_param() {
    if (_xdr_field_number(type_) == 4)
      return check_and_read_param_;
    throw xdr::xdr_wrong_union("ServerPaxosParam: check_and_read_param accessed when not selected");
  }
  const CheckAndReadParam &check_and_read_param() const {
    if (_xdr_field_number(type_) == 4)
      return check_and_read_param_;
    throw xdr::xdr_wrong_union("ServerPaxosParam: check_and_read_param accessed when not selected");
  }
  CheckAndUpdateParam &check_and_update_param() {
    if (_xdr_field_number(type_) == 5)
      return check_and_update_param_;
    throw xdr::xdr_wrong_union("ServerPaxosParam: check_and_update_param accessed when not selected");
  }
  const CheckAndUpdateParam &check_and_update_param() const {
    if (_xdr_field_number(type_) == 5)
      return check_and_update_param_;
    throw xdr::xdr_wrong_union("ServerPaxosParam: check_and_update_param accessed when not selected");
  }
  TestAndSetLockOwnerParam &check_and_test_lock_owner_param() {
    if (_xdr_field_number(type_) == 6)
      return check_and_test_lock_owner_param_;
    throw xdr::xdr_wrong_union("ServerPaxosParam: check_and_test_lock_owner_param accessed when not selected");
  }
  const TestAndSetLockOwnerParam &check_and_test_lock_owner_param() const {
    if (_xdr_field_number(type_) == 6)
      return check_and_test_lock_owner_param_;
    throw xdr::xdr_wrong_union("ServerPaxosParam: check_and_test_lock_owner_param accessed when not selected");
  }
  ResetLockOwnerParam &reset_lock_owner_param() {
    if (_xdr_field_number(type_) == 7)
      return reset_lock_owner_param_;
    throw xdr::xdr_wrong_union("ServerPaxosParam: reset_lock_owner_param accessed when not selected");
  }
  const ResetLockOwnerParam &reset_lock_owner_param() const {
    if (_xdr_field_number(type_) == 7)
      return reset_lock_owner_param_;
    throw xdr::xdr_wrong_union("ServerPaxosParam: reset_lock_owner_param accessed when not selected");
  }
  GetStatesParam &get_states_param() {
    if (_xdr_field_number(type_) == 8)
      return get_states_param_;
    throw xdr::xdr_wrong_union("ServerPaxosParam: get_states_param accessed when not selected");
  }
  const GetStatesParam &get_states_param() const {
    if (_xdr_field_number(type_) == 8)
      return get_states_param_;
    throw xdr::xdr_wrong_union("ServerPaxosParam: get_states_param accessed when not selected");
  }
};
namespace xdr {
template<> struct xdr_traits<::ServerPaxosParam> : xdr_traits_base {
  static constexpr bool is_class = true;
  static constexpr bool is_union = true;
  static constexpr bool has_fixed_size = false;

  using union_type = ::ServerPaxosParam;
  using discriminant_type = decltype(std::declval<union_type>().type());

  static constexpr const char *union_field_name(std::uint32_t which) {
    return which == CHECK_AND_CREATE ? "check_and_create_param"
      : which == CHECK_AND_OPEN ? "check_and_open_param"
      : which == CHECK_AND_DELETE ? "check_and_delete_param"
      : which == CHECK_AND_READ ? "check_and_read_param"
      : which == CHECK_AND_UPDATE ? "check_and_update_param"
      : which == TEST_AND_SET_LOCK_OWNER ? "check_and_test_lock_owner_param"
      : which == RESET_LOCK_OWNER ? "reset_lock_owner_param"
      : which == GET_STATES ? "get_states_param"
      : nullptr;
  }
  static const char *union_field_name(const union_type &u) {
    return union_field_name(u._xdr_discriminant());
  }

  static std::size_t serial_size(const ::ServerPaxosParam &obj) {
    std::size_t size = 0;
    if (!obj._xdr_with_mem_ptr(field_size, obj._xdr_discriminant(), obj, size))
      throw xdr_bad_discriminant("bad value of type in ServerPaxosParam");
    return size + 4;
  }
  template<typename Archive> static void
  save(Archive &ar, const ::ServerPaxosParam &obj) {
    xdr::archive(ar, obj.type(), "type");
    if (!obj._xdr_with_mem_ptr(field_archiver, obj.type(), ar, obj,
                               union_field_name(obj)))
      throw xdr_bad_discriminant("bad value of type in ServerPaxosParam");
  }
  template<typename Archive> static void
  load(Archive &ar, ::ServerPaxosParam &obj) {
    discriminant_type which;
    xdr::archive(ar, which, "type");
    obj.type(which);
    obj._xdr_with_mem_ptr(field_archiver, obj.type(), ar, obj,
                          union_field_name(which));
  }
};
}

struct ServerPaxosResult {
private:
  std::uint32_t type_;
  union {
    CheckAndCreateResult check_and_create_result_;
    CheckAndOpenResult check_and_open_result_;
    CheckAndDeleteResult check_and_delete_result_;
    CheckAndReadResult check_and_read_result_;
    CheckAndUpdateResult check_and_update_result_;
    TestAndSetLockOwnerResult check_and_test_lock_owner_result_;
    ResetLockOwnerResult reset_lock_owner_result_;
    GetStatesResult get_states_result_;
  };

public:
  static_assert (sizeof (unsigned) <= 4, "union discriminant must be 4 bytes");

  static constexpr int _xdr_field_number(std::uint32_t which) {
    return which == CHECK_AND_CREATE ? 1
      : which == CHECK_AND_OPEN ? 2
      : which == CHECK_AND_DELETE ? 3
      : which == CHECK_AND_READ ? 4
      : which == CHECK_AND_UPDATE ? 5
      : which == TEST_AND_SET_LOCK_OWNER ? 6
      : which == RESET_LOCK_OWNER ? 7
      : which == GET_STATES ? 8
      : -1;
  }
  template<typename _F, typename...A> static bool
  _xdr_with_mem_ptr(_F &_f, std::uint32_t which, A&&...a) {
    switch (which) {
    case CHECK_AND_CREATE:
      _f(&ServerPaxosResult::check_and_create_result_, std::forward<A>(a)...);
      return true;
    case CHECK_AND_OPEN:
      _f(&ServerPaxosResult::check_and_open_result_, std::forward<A>(a)...);
      return true;
    case CHECK_AND_DELETE:
      _f(&ServerPaxosResult::check_and_delete_result_, std::forward<A>(a)...);
      return true;
    case CHECK_AND_READ:
      _f(&ServerPaxosResult::check_and_read_result_, std::forward<A>(a)...);
      return true;
    case CHECK_AND_UPDATE:
      _f(&ServerPaxosResult::check_and_update_result_, std::forward<A>(a)...);
      return true;
    case TEST_AND_SET_LOCK_OWNER:
      _f(&ServerPaxosResult::check_and_test_lock_owner_result_, std::forward<A>(a)...);
      return true;
    case RESET_LOCK_OWNER:
      _f(&ServerPaxosResult::reset_lock_owner_result_, std::forward<A>(a)...);
      return true;
    case GET_STATES:
      _f(&ServerPaxosResult::get_states_result_, std::forward<A>(a)...);
      return true;
    }
    return false;
  }

  std::uint32_t _xdr_discriminant() const { return type_; }
  void _xdr_discriminant(std::uint32_t which, bool validate = true) {
    int fnum = _xdr_field_number(which);
    if (fnum < 0 && validate)
      throw xdr::xdr_bad_discriminant("bad value of type in ServerPaxosResult");
    if (fnum != _xdr_field_number(type_)) {
      this->~ServerPaxosResult();
      type_ = which;
      _xdr_with_mem_ptr(xdr::field_constructor, type_, *this);
    }
  }
  ServerPaxosResult(std::uint32_t which = std::uint32_t{}) : type_(which) {
    _xdr_with_mem_ptr(xdr::field_constructor, type_, *this);
  }
  ServerPaxosResult(const ServerPaxosResult &source) : type_(source.type_) {
    _xdr_with_mem_ptr(xdr::field_constructor, type_, *this, source);
  }
  ServerPaxosResult(ServerPaxosResult &&source) : type_(source.type_) {
    _xdr_with_mem_ptr(xdr::field_constructor, type_, *this,
                      std::move(source));
  }
  ~ServerPaxosResult() { _xdr_with_mem_ptr(xdr::field_destructor, type_, *this); }
  ServerPaxosResult &operator=(const ServerPaxosResult &source) {
    if (_xdr_field_number(type_) 
        == _xdr_field_number(source.type_))
      _xdr_with_mem_ptr(xdr::field_assigner, type_, *this, source);
    else {
      this->~ServerPaxosResult();
      type_ = std::uint32_t(-1);
      _xdr_with_mem_ptr(xdr::field_constructor, type_, *this, source);
    }
    type_ = source.type_;
    return *this;
  }
  ServerPaxosResult &operator=(ServerPaxosResult &&source) {
    if (_xdr_field_number(type_)
         == _xdr_field_number(source.type_))
      _xdr_with_mem_ptr(xdr::field_assigner, type_, *this,
                        std::move(source));
    else {
      this->~ServerPaxosResult();
      type_ = std::uint32_t(-1);
      _xdr_with_mem_ptr(xdr::field_constructor, type_, *this,
                        std::move(source));
    }
    type_ = source.type_;
    return *this;
  }

  std::uint32_t type() const { return std::uint32_t(type_); }
  ServerPaxosResult &type(unsigned _xdr_d, bool _xdr_validate = true) {
    _xdr_discriminant(_xdr_d, _xdr_validate);
    return *this;
  }

  CheckAndCreateResult &check_and_create_result() {
    if (_xdr_field_number(type_) == 1)
      return check_and_create_result_;
    throw xdr::xdr_wrong_union("ServerPaxosResult: check_and_create_result accessed when not selected");
  }
  const CheckAndCreateResult &check_and_create_result() const {
    if (_xdr_field_number(type_) == 1)
      return check_and_create_result_;
    throw xdr::xdr_wrong_union("ServerPaxosResult: check_and_create_result accessed when not selected");
  }
  CheckAndOpenResult &check_and_open_result() {
    if (_xdr_field_number(type_) == 2)
      return check_and_open_result_;
    throw xdr::xdr_wrong_union("ServerPaxosResult: check_and_open_result accessed when not selected");
  }
  const CheckAndOpenResult &check_and_open_result() const {
    if (_xdr_field_number(type_) == 2)
      return check_and_open_result_;
    throw xdr::xdr_wrong_union("ServerPaxosResult: check_and_open_result accessed when not selected");
  }
  CheckAndDeleteResult &check_and_delete_result() {
    if (_xdr_field_number(type_) == 3)
      return check_and_delete_result_;
    throw xdr::xdr_wrong_union("ServerPaxosResult: check_and_delete_result accessed when not selected");
  }
  const CheckAndDeleteResult &check_and_delete_result() const {
    if (_xdr_field_number(type_) == 3)
      return check_and_delete_result_;
    throw xdr::xdr_wrong_union("ServerPaxosResult: check_and_delete_result accessed when not selected");
  }
  CheckAndReadResult &check_and_read_result() {
    if (_xdr_field_number(type_) == 4)
      return check_and_read_result_;
    throw xdr::xdr_wrong_union("ServerPaxosResult: check_and_read_result accessed when not selected");
  }
  const CheckAndReadResult &check_and_read_result() const {
    if (_xdr_field_number(type_) == 4)
      return check_and_read_result_;
    throw xdr::xdr_wrong_union("ServerPaxosResult: check_and_read_result accessed when not selected");
  }
  CheckAndUpdateResult &check_and_update_result() {
    if (_xdr_field_number(type_) == 5)
      return check_and_update_result_;
    throw xdr::xdr_wrong_union("ServerPaxosResult: check_and_update_result accessed when not selected");
  }
  const CheckAndUpdateResult &check_and_update_result() const {
    if (_xdr_field_number(type_) == 5)
      return check_and_update_result_;
    throw xdr::xdr_wrong_union("ServerPaxosResult: check_and_update_result accessed when not selected");
  }
  TestAndSetLockOwnerResult &check_and_test_lock_owner_result() {
    if (_xdr_field_number(type_) == 6)
      return check_and_test_lock_owner_result_;
    throw xdr::xdr_wrong_union("ServerPaxosResult: check_and_test_lock_owner_result accessed when not selected");
  }
  const TestAndSetLockOwnerResult &check_and_test_lock_owner_result() const {
    if (_xdr_field_number(type_) == 6)
      return check_and_test_lock_owner_result_;
    throw xdr::xdr_wrong_union("ServerPaxosResult: check_and_test_lock_owner_result accessed when not selected");
  }
  ResetLockOwnerResult &reset_lock_owner_result() {
    if (_xdr_field_number(type_) == 7)
      return reset_lock_owner_result_;
    throw xdr::xdr_wrong_union("ServerPaxosResult: reset_lock_owner_result accessed when not selected");
  }
  const ResetLockOwnerResult &reset_lock_owner_result() const {
    if (_xdr_field_number(type_) == 7)
      return reset_lock_owner_result_;
    throw xdr::xdr_wrong_union("ServerPaxosResult: reset_lock_owner_result accessed when not selected");
  }
  GetStatesResult &get_states_result() {
    if (_xdr_field_number(type_) == 8)
      return get_states_result_;
    throw xdr::xdr_wrong_union("ServerPaxosResult: get_states_result accessed when not selected");
  }
  const GetStatesResult &get_states_result() const {
    if (_xdr_field_number(type_) == 8)
      return get_states_result_;
    throw xdr::xdr_wrong_union("ServerPaxosResult: get_states_result accessed when not selected");
  }
};
namespace xdr {
template<> struct xdr_traits<::ServerPaxosResult> : xdr_traits_base {
  static constexpr bool is_class = true;
  static constexpr bool is_union = true;
  static constexpr bool has_fixed_size = false;

  using union_type = ::ServerPaxosResult;
  using discriminant_type = decltype(std::declval<union_type>().type());

  static constexpr const char *union_field_name(std::uint32_t which) {
    return which == CHECK_AND_CREATE ? "check_and_create_result"
      : which == CHECK_AND_OPEN ? "check_and_open_result"
      : which == CHECK_AND_DELETE ? "check_and_delete_result"
      : which == CHECK_AND_READ ? "check_and_read_result"
      : which == CHECK_AND_UPDATE ? "check_and_update_result"
      : which == TEST_AND_SET_LOCK_OWNER ? "check_and_test_lock_owner_result"
      : which == RESET_LOCK_OWNER ? "reset_lock_owner_result"
      : which == GET_STATES ? "get_states_result"
      : nullptr;
  }
  static const char *union_field_name(const union_type &u) {
    return union_field_name(u._xdr_discriminant());
  }

  static std::size_t serial_size(const ::ServerPaxosResult &obj) {
    std::size_t size = 0;
    if (!obj._xdr_with_mem_ptr(field_size, obj._xdr_discriminant(), obj, size))
      throw xdr_bad_discriminant("bad value of type in ServerPaxosResult");
    return size + 4;
  }
  template<typename Archive> static void
  save(Archive &ar, const ::ServerPaxosResult &obj) {
    xdr::archive(ar, obj.type(), "type");
    if (!obj._xdr_with_mem_ptr(field_archiver, obj.type(), ar, obj,
                               union_field_name(obj)))
      throw xdr_bad_discriminant("bad value of type in ServerPaxosResult");
  }
  template<typename Archive> static void
  load(Archive &ar, ::ServerPaxosResult &obj) {
    discriminant_type which;
    xdr::archive(ar, which, "type");
    obj.type(which);
    obj._xdr_with_mem_ptr(field_archiver, obj.type(), ar, obj,
                          union_field_name(which));
  }
};
}

#endif // !__XDR_SERVER_PAXOS_HH_INCLUDED__
