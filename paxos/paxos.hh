// -*- C++ -*-
// Automatically generated from paxos/paxos.x.
// DO NOT EDIT or your changes may be overwritten

#ifndef __XDR_PAXOS_PAXOS_HH_INCLUDED__
#define __XDR_PAXOS_PAXOS_HH_INCLUDED__ 1

#include <xdrpp/types.h>

using cid_t = xdr::xstring<>;
using uid_t = std::uint32_t;
using net_address_t = xdr::xstring<>;

struct viewid_t {
  std::uint64_t counter{};
  cid_t manager{};
};
namespace xdr {
template<> struct xdr_traits<::viewid_t>
  : xdr_struct_base<field_ptr<::viewid_t,
                              decltype(::viewid_t::counter),
                              &::viewid_t::counter>,
                    field_ptr<::viewid_t,
                              decltype(::viewid_t::manager),
                              &::viewid_t::manager>> {
  template<typename Archive> static void
  save(Archive &ar, const ::viewid_t &obj) {
    archive(ar, obj.counter, "counter");
    archive(ar, obj.manager, "manager");
  }
  template<typename Archive> static void
  load(Archive &ar, ::viewid_t &obj) {
    archive(ar, obj.counter, "counter");
    archive(ar, obj.manager, "manager");
  }
};
}

struct viewstamp_t {
  viewid_t vid{};
  std::uint32_t ts{};
};
namespace xdr {
template<> struct xdr_traits<::viewstamp_t>
  : xdr_struct_base<field_ptr<::viewstamp_t,
                              decltype(::viewstamp_t::vid),
                              &::viewstamp_t::vid>,
                    field_ptr<::viewstamp_t,
                              decltype(::viewstamp_t::ts),
                              &::viewstamp_t::ts>> {
  template<typename Archive> static void
  save(Archive &ar, const ::viewstamp_t &obj) {
    archive(ar, obj.vid, "vid");
    archive(ar, obj.ts, "ts");
  }
  template<typename Archive> static void
  load(Archive &ar, ::viewstamp_t &obj) {
    archive(ar, obj.vid, "vid");
    archive(ar, obj.ts, "ts");
  }
};
}

struct cohort_t {
  cid_t id{};
  net_address_t addr{};
};
namespace xdr {
template<> struct xdr_traits<::cohort_t>
  : xdr_struct_base<field_ptr<::cohort_t,
                              decltype(::cohort_t::id),
                              &::cohort_t::id>,
                    field_ptr<::cohort_t,
                              decltype(::cohort_t::addr),
                              &::cohort_t::addr>> {
  template<typename Archive> static void
  save(Archive &ar, const ::cohort_t &obj) {
    archive(ar, obj.id, "id");
    archive(ar, obj.addr, "addr");
  }
  template<typename Archive> static void
  load(Archive &ar, ::cohort_t &obj) {
    archive(ar, obj.id, "id");
    archive(ar, obj.addr, "addr");
  }
};
}

struct view_t {
  viewid_t vid{};
  cohort_t primary{};
  xdr::xvector<cohort_t> backups{};
};
namespace xdr {
template<> struct xdr_traits<::view_t>
  : xdr_struct_base<field_ptr<::view_t,
                              decltype(::view_t::vid),
                              &::view_t::vid>,
                    field_ptr<::view_t,
                              decltype(::view_t::primary),
                              &::view_t::primary>,
                    field_ptr<::view_t,
                              decltype(::view_t::backups),
                              &::view_t::backups>> {
  template<typename Archive> static void
  save(Archive &ar, const ::view_t &obj) {
    archive(ar, obj.vid, "vid");
    archive(ar, obj.primary, "primary");
    archive(ar, obj.backups, "backups");
  }
  template<typename Archive> static void
  load(Archive &ar, ::view_t &obj) {
    archive(ar, obj.vid, "vid");
    archive(ar, obj.primary, "primary");
    archive(ar, obj.backups, "backups");
  }
};
}

struct execute_arg {
  cid_t cid{};
  uid_t rid{};
  viewid_t vid{};
  xdr::opaque_vec<> request{};
};
namespace xdr {
template<> struct xdr_traits<::execute_arg>
  : xdr_struct_base<field_ptr<::execute_arg,
                              decltype(::execute_arg::cid),
                              &::execute_arg::cid>,
                    field_ptr<::execute_arg,
                              decltype(::execute_arg::rid),
                              &::execute_arg::rid>,
                    field_ptr<::execute_arg,
                              decltype(::execute_arg::vid),
                              &::execute_arg::vid>,
                    field_ptr<::execute_arg,
                              decltype(::execute_arg::request),
                              &::execute_arg::request>> {
  template<typename Archive> static void
  save(Archive &ar, const ::execute_arg &obj) {
    archive(ar, obj.cid, "cid");
    archive(ar, obj.rid, "rid");
    archive(ar, obj.vid, "vid");
    archive(ar, obj.request, "request");
  }
  template<typename Archive> static void
  load(Archive &ar, ::execute_arg &obj) {
    archive(ar, obj.cid, "cid");
    archive(ar, obj.rid, "rid");
    archive(ar, obj.vid, "vid");
    archive(ar, obj.request, "request");
  }
};
}

struct replicate_arg {
  viewstamp_t vs{};
  execute_arg arg{};
  viewstamp_t committed{};
};
namespace xdr {
template<> struct xdr_traits<::replicate_arg>
  : xdr_struct_base<field_ptr<::replicate_arg,
                              decltype(::replicate_arg::vs),
                              &::replicate_arg::vs>,
                    field_ptr<::replicate_arg,
                              decltype(::replicate_arg::arg),
                              &::replicate_arg::arg>,
                    field_ptr<::replicate_arg,
                              decltype(::replicate_arg::committed),
                              &::replicate_arg::committed>> {
  template<typename Archive> static void
  save(Archive &ar, const ::replicate_arg &obj) {
    archive(ar, obj.vs, "vs");
    archive(ar, obj.arg, "arg");
    archive(ar, obj.committed, "committed");
  }
  template<typename Archive> static void
  load(Archive &ar, ::replicate_arg &obj) {
    archive(ar, obj.vs, "vs");
    archive(ar, obj.arg, "arg");
    archive(ar, obj.committed, "committed");
  }
};
}

struct replicate_res {
  viewstamp_t vs{};
};
namespace xdr {
template<> struct xdr_traits<::replicate_res>
  : xdr_struct_base<field_ptr<::replicate_res,
                              decltype(::replicate_res::vs),
                              &::replicate_res::vs>> {
  template<typename Archive> static void
  save(Archive &ar, const ::replicate_res &obj) {
    archive(ar, obj.vs, "vs");
  }
  template<typename Archive> static void
  load(Archive &ar, ::replicate_res &obj) {
    archive(ar, obj.vs, "vs");
  }
};
}

struct execute_viewinfo {
  viewid_t vid{};
  net_address_t primary{};
};
namespace xdr {
template<> struct xdr_traits<::execute_viewinfo>
  : xdr_struct_base<field_ptr<::execute_viewinfo,
                              decltype(::execute_viewinfo::vid),
                              &::execute_viewinfo::vid>,
                    field_ptr<::execute_viewinfo,
                              decltype(::execute_viewinfo::primary),
                              &::execute_viewinfo::primary>> {
  template<typename Archive> static void
  save(Archive &ar, const ::execute_viewinfo &obj) {
    archive(ar, obj.vid, "vid");
    archive(ar, obj.primary, "primary");
  }
  template<typename Archive> static void
  load(Archive &ar, ::execute_viewinfo &obj) {
    archive(ar, obj.vid, "vid");
    archive(ar, obj.primary, "primary");
  }
};
}

struct execute_res {
private:
  std::uint32_t ok_;
  union {
    xdr::opaque_vec<> reply_;
    execute_viewinfo viewinfo_;
  };

public:
  static_assert (sizeof (bool) <= 4, "union discriminant must be 4 bytes");

  static constexpr int _xdr_field_number(std::uint32_t which) {
    return which == true ? 1
      : which == false ? 2
      : -1;
  }
  template<typename _F, typename...A> static bool
  _xdr_with_mem_ptr(_F &_f, std::uint32_t which, A&&...a) {
    switch (std::uint32_t{which}) {
    case true:
      _f(&execute_res::reply_, std::forward<A>(a)...);
      return true;
    case false:
      _f(&execute_res::viewinfo_, std::forward<A>(a)...);
      return true;
    }
    return false;
  }

  std::uint32_t _xdr_discriminant() const { return ok_; }
  void _xdr_discriminant(std::uint32_t which, bool validate = true) {
    int fnum = _xdr_field_number(which);
    if (fnum < 0 && validate)
      throw xdr::xdr_bad_discriminant("bad value of ok in execute_res");
    if (fnum != _xdr_field_number(ok_)) {
      this->~execute_res();
      ok_ = which;
      _xdr_with_mem_ptr(xdr::field_constructor, ok_, *this);
    }
  }
  execute_res(bool which = bool{}) : ok_(which) {
    _xdr_with_mem_ptr(xdr::field_constructor, ok_, *this);
  }
  execute_res(const execute_res &source) : ok_(source.ok_) {
    _xdr_with_mem_ptr(xdr::field_constructor, ok_, *this, source);
  }
  execute_res(execute_res &&source) : ok_(source.ok_) {
    _xdr_with_mem_ptr(xdr::field_constructor, ok_, *this,
                      std::move(source));
  }
  ~execute_res() { _xdr_with_mem_ptr(xdr::field_destructor, ok_, *this); }
  execute_res &operator=(const execute_res &source) {
    if (_xdr_field_number(ok_) 
        == _xdr_field_number(source.ok_))
      _xdr_with_mem_ptr(xdr::field_assigner, ok_, *this, source);
    else {
      this->~execute_res();
      ok_ = std::uint32_t(-1);
      _xdr_with_mem_ptr(xdr::field_constructor, ok_, *this, source);
    }
    ok_ = source.ok_;
    return *this;
  }
  execute_res &operator=(execute_res &&source) {
    if (_xdr_field_number(ok_)
         == _xdr_field_number(source.ok_))
      _xdr_with_mem_ptr(xdr::field_assigner, ok_, *this,
                        std::move(source));
    else {
      this->~execute_res();
      ok_ = std::uint32_t(-1);
      _xdr_with_mem_ptr(xdr::field_constructor, ok_, *this,
                        std::move(source));
    }
    ok_ = source.ok_;
    return *this;
  }

  bool ok() const { return bool(ok_); }
  execute_res &ok(bool _xdr_d, bool _xdr_validate = true) {
    _xdr_discriminant(_xdr_d, _xdr_validate);
    return *this;
  }

  xdr::opaque_vec<> &reply() {
    if (_xdr_field_number(ok_) == 1)
      return reply_;
    throw xdr::xdr_wrong_union("execute_res: reply accessed when not selected");
  }
  const xdr::opaque_vec<> &reply() const {
    if (_xdr_field_number(ok_) == 1)
      return reply_;
    throw xdr::xdr_wrong_union("execute_res: reply accessed when not selected");
  }
  execute_viewinfo &viewinfo() {
    if (_xdr_field_number(ok_) == 2)
      return viewinfo_;
    throw xdr::xdr_wrong_union("execute_res: viewinfo accessed when not selected");
  }
  const execute_viewinfo &viewinfo() const {
    if (_xdr_field_number(ok_) == 2)
      return viewinfo_;
    throw xdr::xdr_wrong_union("execute_res: viewinfo accessed when not selected");
  }
};
namespace xdr {
template<> struct xdr_traits<::execute_res> : xdr_traits_base {
  static constexpr bool is_class = true;
  static constexpr bool is_union = true;
  static constexpr bool has_fixed_size = false;

  using union_type = ::execute_res;
  using discriminant_type = decltype(std::declval<union_type>().ok());

  static constexpr const char *union_field_name(std::uint32_t which) {
    return which == true ? "reply"
      : which == false ? "viewinfo"
      : nullptr;
  }
  static const char *union_field_name(const union_type &u) {
    return union_field_name(u._xdr_discriminant());
  }

  static std::size_t serial_size(const ::execute_res &obj) {
    std::size_t size = 0;
    if (!obj._xdr_with_mem_ptr(field_size, obj._xdr_discriminant(), obj, size))
      throw xdr_bad_discriminant("bad value of ok in execute_res");
    return size + 4;
  }
  template<typename Archive> static void
  save(Archive &ar, const ::execute_res &obj) {
    xdr::archive(ar, obj.ok(), "ok");
    if (!obj._xdr_with_mem_ptr(field_archiver, obj.ok(), ar, obj,
                               union_field_name(obj)))
      throw xdr_bad_discriminant("bad value of ok in execute_res");
  }
  template<typename Archive> static void
  load(Archive &ar, ::execute_res &obj) {
    discriminant_type which;
    xdr::archive(ar, which, "ok");
    obj.ok(which);
    obj._xdr_with_mem_ptr(field_archiver, obj.ok(), ar, obj,
                          union_field_name(which));
  }
};
}

struct vc_state {
  enum _mode_t : std::uint32_t {
    VC_ACTIVE,
    VC_MANAGER,
    VC_UNDERLING,
  };

  _mode_t mode{};
  view_t view{};
};
namespace xdr {
template<> struct xdr_traits<::vc_state::_mode_t>
  : xdr_integral_base<::vc_state::_mode_t, std::uint32_t> {
  static constexpr bool is_enum = true;
  static constexpr bool is_numeric = false;
  static const char *enum_name(::vc_state::_mode_t val) {
    switch (val) {
    case ::vc_state::VC_ACTIVE:
      return "VC_ACTIVE";
    case ::vc_state::VC_MANAGER:
      return "VC_MANAGER";
    case ::vc_state::VC_UNDERLING:
      return "VC_UNDERLING";
    default:
      return nullptr;
    }
  }
};
template<> struct xdr_traits<::vc_state>
  : xdr_struct_base<field_ptr<::vc_state,
                              decltype(::vc_state::mode),
                              &::vc_state::mode>,
                    field_ptr<::vc_state,
                              decltype(::vc_state::view),
                              &::vc_state::view>> {
  template<typename Archive> static void
  save(Archive &ar, const ::vc_state &obj) {
    archive(ar, obj.mode, "mode");
    archive(ar, obj.view, "view");
  }
  template<typename Archive> static void
  load(Archive &ar, ::vc_state &obj) {
    archive(ar, obj.mode, "mode");
    archive(ar, obj.view, "view");
  }
};
}

struct view_change_arg {
  view_t oldview{};
  viewid_t newvid{};
};
namespace xdr {
template<> struct xdr_traits<::view_change_arg>
  : xdr_struct_base<field_ptr<::view_change_arg,
                              decltype(::view_change_arg::oldview),
                              &::view_change_arg::oldview>,
                    field_ptr<::view_change_arg,
                              decltype(::view_change_arg::newvid),
                              &::view_change_arg::newvid>> {
  template<typename Archive> static void
  save(Archive &ar, const ::view_change_arg &obj) {
    archive(ar, obj.oldview, "oldview");
    archive(ar, obj.newvid, "newvid");
  }
  template<typename Archive> static void
  load(Archive &ar, ::view_change_arg &obj) {
    archive(ar, obj.oldview, "oldview");
    archive(ar, obj.newvid, "newvid");
  }
};
}

struct view_change_reject {
  view_t oldview{};
  viewid_t newid{};
};
namespace xdr {
template<> struct xdr_traits<::view_change_reject>
  : xdr_struct_base<field_ptr<::view_change_reject,
                              decltype(::view_change_reject::oldview),
                              &::view_change_reject::oldview>,
                    field_ptr<::view_change_reject,
                              decltype(::view_change_reject::newid),
                              &::view_change_reject::newid>> {
  template<typename Archive> static void
  save(Archive &ar, const ::view_change_reject &obj) {
    archive(ar, obj.oldview, "oldview");
    archive(ar, obj.newid, "newid");
  }
  template<typename Archive> static void
  load(Archive &ar, ::view_change_reject &obj) {
    archive(ar, obj.oldview, "oldview");
    archive(ar, obj.newid, "newid");
  }
};
}

struct view_change_accept {
  cid_t myid{};
  viewstamp_t latest{};
  view_t newview{};
};
namespace xdr {
template<> struct xdr_traits<::view_change_accept>
  : xdr_struct_base<field_ptr<::view_change_accept,
                              decltype(::view_change_accept::myid),
                              &::view_change_accept::myid>,
                    field_ptr<::view_change_accept,
                              decltype(::view_change_accept::latest),
                              &::view_change_accept::latest>,
                    field_ptr<::view_change_accept,
                              decltype(::view_change_accept::newview),
                              &::view_change_accept::newview>> {
  template<typename Archive> static void
  save(Archive &ar, const ::view_change_accept &obj) {
    archive(ar, obj.myid, "myid");
    archive(ar, obj.latest, "latest");
    archive(ar, obj.newview, "newview");
  }
  template<typename Archive> static void
  load(Archive &ar, ::view_change_accept &obj) {
    archive(ar, obj.myid, "myid");
    archive(ar, obj.latest, "latest");
    archive(ar, obj.newview, "newview");
  }
};
}

struct view_change_res {
private:
  std::uint32_t accepted_;
  union {
    view_change_reject reject_;
    view_change_accept accept_;
  };

public:
  static_assert (sizeof (bool) <= 4, "union discriminant must be 4 bytes");

  static constexpr int _xdr_field_number(std::uint32_t which) {
    return which == false ? 1
      : which == true ? 2
      : -1;
  }
  template<typename _F, typename...A> static bool
  _xdr_with_mem_ptr(_F &_f, std::uint32_t which, A&&...a) {
    switch (std::uint32_t{which}) {
    case false:
      _f(&view_change_res::reject_, std::forward<A>(a)...);
      return true;
    case true:
      _f(&view_change_res::accept_, std::forward<A>(a)...);
      return true;
    }
    return false;
  }

  std::uint32_t _xdr_discriminant() const { return accepted_; }
  void _xdr_discriminant(std::uint32_t which, bool validate = true) {
    int fnum = _xdr_field_number(which);
    if (fnum < 0 && validate)
      throw xdr::xdr_bad_discriminant("bad value of accepted in view_change_res");
    if (fnum != _xdr_field_number(accepted_)) {
      this->~view_change_res();
      accepted_ = which;
      _xdr_with_mem_ptr(xdr::field_constructor, accepted_, *this);
    }
  }
  view_change_res(bool which = bool{}) : accepted_(which) {
    _xdr_with_mem_ptr(xdr::field_constructor, accepted_, *this);
  }
  view_change_res(const view_change_res &source) : accepted_(source.accepted_) {
    _xdr_with_mem_ptr(xdr::field_constructor, accepted_, *this, source);
  }
  view_change_res(view_change_res &&source) : accepted_(source.accepted_) {
    _xdr_with_mem_ptr(xdr::field_constructor, accepted_, *this,
                      std::move(source));
  }
  ~view_change_res() { _xdr_with_mem_ptr(xdr::field_destructor, accepted_, *this); }
  view_change_res &operator=(const view_change_res &source) {
    if (_xdr_field_number(accepted_) 
        == _xdr_field_number(source.accepted_))
      _xdr_with_mem_ptr(xdr::field_assigner, accepted_, *this, source);
    else {
      this->~view_change_res();
      accepted_ = std::uint32_t(-1);
      _xdr_with_mem_ptr(xdr::field_constructor, accepted_, *this, source);
    }
    accepted_ = source.accepted_;
    return *this;
  }
  view_change_res &operator=(view_change_res &&source) {
    if (_xdr_field_number(accepted_)
         == _xdr_field_number(source.accepted_))
      _xdr_with_mem_ptr(xdr::field_assigner, accepted_, *this,
                        std::move(source));
    else {
      this->~view_change_res();
      accepted_ = std::uint32_t(-1);
      _xdr_with_mem_ptr(xdr::field_constructor, accepted_, *this,
                        std::move(source));
    }
    accepted_ = source.accepted_;
    return *this;
  }

  bool accepted() const { return bool(accepted_); }
  view_change_res &accepted(bool _xdr_d, bool _xdr_validate = true) {
    _xdr_discriminant(_xdr_d, _xdr_validate);
    return *this;
  }

  view_change_reject &reject() {
    if (_xdr_field_number(accepted_) == 1)
      return reject_;
    throw xdr::xdr_wrong_union("view_change_res: reject accessed when not selected");
  }
  const view_change_reject &reject() const {
    if (_xdr_field_number(accepted_) == 1)
      return reject_;
    throw xdr::xdr_wrong_union("view_change_res: reject accessed when not selected");
  }
  view_change_accept &accept() {
    if (_xdr_field_number(accepted_) == 2)
      return accept_;
    throw xdr::xdr_wrong_union("view_change_res: accept accessed when not selected");
  }
  const view_change_accept &accept() const {
    if (_xdr_field_number(accepted_) == 2)
      return accept_;
    throw xdr::xdr_wrong_union("view_change_res: accept accessed when not selected");
  }
};
namespace xdr {
template<> struct xdr_traits<::view_change_res> : xdr_traits_base {
  static constexpr bool is_class = true;
  static constexpr bool is_union = true;
  static constexpr bool has_fixed_size = false;

  using union_type = ::view_change_res;
  using discriminant_type = decltype(std::declval<union_type>().accepted());

  static constexpr const char *union_field_name(std::uint32_t which) {
    return which == false ? "reject"
      : which == true ? "accept"
      : nullptr;
  }
  static const char *union_field_name(const union_type &u) {
    return union_field_name(u._xdr_discriminant());
  }

  static std::size_t serial_size(const ::view_change_res &obj) {
    std::size_t size = 0;
    if (!obj._xdr_with_mem_ptr(field_size, obj._xdr_discriminant(), obj, size))
      throw xdr_bad_discriminant("bad value of accepted in view_change_res");
    return size + 4;
  }
  template<typename Archive> static void
  save(Archive &ar, const ::view_change_res &obj) {
    xdr::archive(ar, obj.accepted(), "accepted");
    if (!obj._xdr_with_mem_ptr(field_archiver, obj.accepted(), ar, obj,
                               union_field_name(obj)))
      throw xdr_bad_discriminant("bad value of accepted in view_change_res");
  }
  template<typename Archive> static void
  load(Archive &ar, ::view_change_res &obj) {
    discriminant_type which;
    xdr::archive(ar, which, "accepted");
    obj.accepted(which);
    obj._xdr_with_mem_ptr(field_archiver, obj.accepted(), ar, obj,
                          union_field_name(which));
  }
};
}

struct new_view_arg {
  viewstamp_t latest{};
  view_t view{};
};
namespace xdr {
template<> struct xdr_traits<::new_view_arg>
  : xdr_struct_base<field_ptr<::new_view_arg,
                              decltype(::new_view_arg::latest),
                              &::new_view_arg::latest>,
                    field_ptr<::new_view_arg,
                              decltype(::new_view_arg::view),
                              &::new_view_arg::view>> {
  template<typename Archive> static void
  save(Archive &ar, const ::new_view_arg &obj) {
    archive(ar, obj.latest, "latest");
    archive(ar, obj.view, "view");
  }
  template<typename Archive> static void
  load(Archive &ar, ::new_view_arg &obj) {
    archive(ar, obj.latest, "latest");
    archive(ar, obj.view, "view");
  }
};
}

struct new_view_res {
  bool accepted{};
};
namespace xdr {
template<> struct xdr_traits<::new_view_res>
  : xdr_struct_base<field_ptr<::new_view_res,
                              decltype(::new_view_res::accepted),
                              &::new_view_res::accepted>> {
  template<typename Archive> static void
  save(Archive &ar, const ::new_view_res &obj) {
    archive(ar, obj.accepted, "accepted");
  }
  template<typename Archive> static void
  load(Archive &ar, ::new_view_res &obj) {
    archive(ar, obj.accepted, "accepted");
  }
};
}

struct init_view_arg {
  view_t view{};
};
namespace xdr {
template<> struct xdr_traits<::init_view_arg>
  : xdr_struct_base<field_ptr<::init_view_arg,
                              decltype(::init_view_arg::view),
                              &::init_view_arg::view>> {
  template<typename Archive> static void
  save(Archive &ar, const ::init_view_arg &obj) {
    archive(ar, obj.view, "view");
  }
  template<typename Archive> static void
  load(Archive &ar, ::init_view_arg &obj) {
    archive(ar, obj.view, "view");
  }
};
}

struct init_view_res {
  bool succeed{};
};
namespace xdr {
template<> struct xdr_traits<::init_view_res>
  : xdr_struct_base<field_ptr<::init_view_res,
                              decltype(::init_view_res::succeed),
                              &::init_view_res::succeed>> {
  template<typename Archive> static void
  save(Archive &ar, const ::init_view_res &obj) {
    archive(ar, obj.succeed, "succeed");
  }
  template<typename Archive> static void
  load(Archive &ar, ::init_view_res &obj) {
    archive(ar, obj.succeed, "succeed");
  }
};
}

struct init_view_request {
  view_t newview{};
  viewstamp_t prev{};
};
namespace xdr {
template<> struct xdr_traits<::init_view_request>
  : xdr_struct_base<field_ptr<::init_view_request,
                              decltype(::init_view_request::newview),
                              &::init_view_request::newview>,
                    field_ptr<::init_view_request,
                              decltype(::init_view_request::prev),
                              &::init_view_request::prev>> {
  template<typename Archive> static void
  save(Archive &ar, const ::init_view_request &obj) {
    archive(ar, obj.newview, "newview");
    archive(ar, obj.prev, "prev");
  }
  template<typename Archive> static void
  load(Archive &ar, ::init_view_request &obj) {
    archive(ar, obj.newview, "newview");
    archive(ar, obj.prev, "prev");
  }
};
}

struct paxos_v1 {
  static constexpr std::uint32_t program = 805531765;
  static constexpr const char *program_name = "paxos_interface";
  static constexpr std::uint32_t version = 1;
  static constexpr const char *version_name = "paxos_v1";

  struct replicate_t {
    using interface_type = paxos_v1;
    static constexpr std::uint32_t proc = 2;
    static constexpr const char *proc_name = "replicate";
    using arg_type = replicate_arg;
    using arg_wire_type = replicate_arg;
    using res_type = replicate_res;
    using res_wire_type = replicate_res;
    
    template<typename C, typename...A> static auto
    dispatch(C &&c, A &&...a) ->
    decltype(c.replicate(std::forward<A>(a)...)) {
      return c.replicate(std::forward<A>(a)...);
    }
    
    template<typename C, typename DropIfVoid, typename...A> static auto
    dispatch_dropvoid(C &&c, DropIfVoid &&d, A &&...a) ->
    decltype(c.replicate(std::forward<DropIfVoid>(d), std::forward<A>(a)...)) {
      return c.replicate(std::forward<DropIfVoid>(d), std::forward<A>(a)...);
    }
  };

  struct view_change_t {
    using interface_type = paxos_v1;
    static constexpr std::uint32_t proc = 101;
    static constexpr const char *proc_name = "view_change";
    using arg_type = view_change_arg;
    using arg_wire_type = view_change_arg;
    using res_type = view_change_res;
    using res_wire_type = view_change_res;
    
    template<typename C, typename...A> static auto
    dispatch(C &&c, A &&...a) ->
    decltype(c.view_change(std::forward<A>(a)...)) {
      return c.view_change(std::forward<A>(a)...);
    }
    
    template<typename C, typename DropIfVoid, typename...A> static auto
    dispatch_dropvoid(C &&c, DropIfVoid &&d, A &&...a) ->
    decltype(c.view_change(std::forward<DropIfVoid>(d), std::forward<A>(a)...)) {
      return c.view_change(std::forward<DropIfVoid>(d), std::forward<A>(a)...);
    }
  };

  struct new_view_t {
    using interface_type = paxos_v1;
    static constexpr std::uint32_t proc = 102;
    static constexpr const char *proc_name = "new_view";
    using arg_type = new_view_arg;
    using arg_wire_type = new_view_arg;
    using res_type = new_view_res;
    using res_wire_type = new_view_res;
    
    template<typename C, typename...A> static auto
    dispatch(C &&c, A &&...a) ->
    decltype(c.new_view(std::forward<A>(a)...)) {
      return c.new_view(std::forward<A>(a)...);
    }
    
    template<typename C, typename DropIfVoid, typename...A> static auto
    dispatch_dropvoid(C &&c, DropIfVoid &&d, A &&...a) ->
    decltype(c.new_view(std::forward<DropIfVoid>(d), std::forward<A>(a)...)) {
      return c.new_view(std::forward<DropIfVoid>(d), std::forward<A>(a)...);
    }
  };

  struct init_view_t {
    using interface_type = paxos_v1;
    static constexpr std::uint32_t proc = 103;
    static constexpr const char *proc_name = "init_view";
    using arg_type = init_view_arg;
    using arg_wire_type = init_view_arg;
    using res_type = init_view_res;
    using res_wire_type = init_view_res;
    
    template<typename C, typename...A> static auto
    dispatch(C &&c, A &&...a) ->
    decltype(c.init_view(std::forward<A>(a)...)) {
      return c.init_view(std::forward<A>(a)...);
    }
    
    template<typename C, typename DropIfVoid, typename...A> static auto
    dispatch_dropvoid(C &&c, DropIfVoid &&d, A &&...a) ->
    decltype(c.init_view(std::forward<DropIfVoid>(d), std::forward<A>(a)...)) {
      return c.init_view(std::forward<DropIfVoid>(d), std::forward<A>(a)...);
    }
  };

  template<typename T, typename...A> static bool
  call_dispatch(T &&t, std::uint32_t proc, A &&...a) {
    switch(proc) {
    case 2:
      t.template dispatch<replicate_t>(std::forward<A>(a)...);
      return true;
    case 101:
      t.template dispatch<view_change_t>(std::forward<A>(a)...);
      return true;
    case 102:
      t.template dispatch<new_view_t>(std::forward<A>(a)...);
      return true;
    case 103:
      t.template dispatch<init_view_t>(std::forward<A>(a)...);
      return true;
    }
    return false;
  }

  template<typename _XDRBASE> struct client : _XDRBASE {
    using _XDRBASE::_XDRBASE;

    template<typename..._XDRARGS> auto
    replicate(_XDRARGS &&..._xdr_args) ->
    decltype(this->_XDRBASE::template invoke<replicate_t>(_xdr_args...)) {
      return this->_XDRBASE::template invoke<replicate_t>(_xdr_args...);
    }

    template<typename..._XDRARGS> auto
    view_change(_XDRARGS &&..._xdr_args) ->
    decltype(this->_XDRBASE::template invoke<view_change_t>(_xdr_args...)) {
      return this->_XDRBASE::template invoke<view_change_t>(_xdr_args...);
    }

    template<typename..._XDRARGS> auto
    new_view(_XDRARGS &&..._xdr_args) ->
    decltype(this->_XDRBASE::template invoke<new_view_t>(_xdr_args...)) {
      return this->_XDRBASE::template invoke<new_view_t>(_xdr_args...);
    }

    template<typename..._XDRARGS> auto
    init_view(_XDRARGS &&..._xdr_args) ->
    decltype(this->_XDRBASE::template invoke<init_view_t>(_xdr_args...)) {
      return this->_XDRBASE::template invoke<init_view_t>(_xdr_args...);
    }
  };
};

struct paxos_client_v1 {
  static constexpr std::uint32_t program = 805531765;
  static constexpr const char *program_name = "paxos_interface";
  static constexpr std::uint32_t version = 2;
  static constexpr const char *version_name = "paxos_client_v1";

  struct execute_t {
    using interface_type = paxos_client_v1;
    static constexpr std::uint32_t proc = 1;
    static constexpr const char *proc_name = "execute";
    using arg_type = execute_arg;
    using arg_wire_type = execute_arg;
    using res_type = execute_res;
    using res_wire_type = execute_res;
    
    template<typename C, typename...A> static auto
    dispatch(C &&c, A &&...a) ->
    decltype(c.execute(std::forward<A>(a)...)) {
      return c.execute(std::forward<A>(a)...);
    }
    
    template<typename C, typename DropIfVoid, typename...A> static auto
    dispatch_dropvoid(C &&c, DropIfVoid &&d, A &&...a) ->
    decltype(c.execute(std::forward<DropIfVoid>(d), std::forward<A>(a)...)) {
      return c.execute(std::forward<DropIfVoid>(d), std::forward<A>(a)...);
    }
  };

  template<typename T, typename...A> static bool
  call_dispatch(T &&t, std::uint32_t proc, A &&...a) {
    switch(proc) {
    case 1:
      t.template dispatch<execute_t>(std::forward<A>(a)...);
      return true;
    }
    return false;
  }

  template<typename _XDRBASE> struct client : _XDRBASE {
    using _XDRBASE::_XDRBASE;

    template<typename..._XDRARGS> auto
    execute(_XDRARGS &&..._xdr_args) ->
    decltype(this->_XDRBASE::template invoke<execute_t>(_xdr_args...)) {
      return this->_XDRBASE::template invoke<execute_t>(_xdr_args...);
    }
  };
};

#endif // !__XDR_PAXOS_PAXOS_HH_INCLUDED__
