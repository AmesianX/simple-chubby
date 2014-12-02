/*
Auxiliary data structures.
*/
// The client's unique identifier.
typedef string cid_t<>;
// The unique identifier for a request.
typedef unsigned uid_t;
// The network address.
typedef string net_address_t<>;

/*
Types to describe a view.
*/
struct viewid_t {
  unsigned hyper counter;
  cid_t manager;
};

struct viewstamp_t {
  viewid_t vid;
  unsigned ts;
};

struct cohort_t {
  cid_t id;
  net_address_t addr;
};

struct view_t {
  viewid_t vid;
  cohort_t primary;
  cohort_t backups<>; 
};

/*
The normal process:
"execute_arg" is received by the leader.
"replicate_arg" is broadcast to all the replicas.
"replicate_res" is sent back to the leader.
*/

struct execute_arg {
  cid_t cid;
  uid_t rid;
  viewid_t vid;
  opaque request<>;
};

struct replicate_arg {
  viewstamp_t vs;
  execute_arg arg;
  // opaque extra<>;
  viewstamp_t committed;
};

struct replicate_res {
  viewstamp_t vs;
};

struct execute_viewinfo {
  viewid_t vid;
  net_address_t primary;
};
union execute_res switch (bool ok) {
  case TRUE:
    opaque reply<>;
  case FALSE:
    execute_viewinfo viewinfo;
};

/*
The view state of a replica.
*/
struct vc_state {
  enum {
    // active in a formed view.
    VC_ACTIVE,
    // proposing a new view.
    VC_MANAGER,
    // invited to join a new view.
    VC_UNDERLING
  } mode;
  // last (or current) formed view.
  view_t view;
};

/*
View changing process:
"view_change_arg" is broadcast by the view manager.
"view_change_res" is sent back.
"new_view_arg" is broadcast by the view manager.
"new_view_res" is sent back.
"init_view_arg" is sent from the view manager to the leader.
A special "replicate_arg" encoding "init_view_request" is broadcast.
*/
struct view_change_arg {
  view_t oldview;
  viewid_t newvid;
};

struct view_change_reject {
  view_t oldview;
  viewid_t newid;
};
struct view_change_accept {
  cid_t myid;
  // bool include_me;
  viewstamp_t latest;
  // view_t *newview;
  view_t newview;
};
union view_change_res
    switch (bool accepted) {
  case FALSE:
    view_change_reject reject;
  case TRUE:
    view_change_accept accept;
};

struct new_view_arg {
  viewstamp_t latest;
  view_t view;
};

struct new_view_res {
  bool accepted;
};

struct init_view_arg {
  view_t view;
};

struct init_view_res {
  bool succeed;
};

struct init_view_request {
  view_t newview;
  viewstamp_t prev;
};

program paxos_interface {
  version paxos_v1 {
    /* Normal process. */
    execute_res execute(execute_arg) = 1;
    replicate_res replicate(replicate_arg) = 2;
    /* View changing process. */
    view_change_res view_change(view_change_arg) = 101;
    new_view_res new_view(new_view_arg) = 102;
    init_view_res init_view(init_view_arg) = 103;
    /* a special replicate followed. */
  } = 1;
} = 0x30037075;

