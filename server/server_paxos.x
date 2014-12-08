enum CallType {
CHECK_AND_CREATE,
CHECK_AND_OPEN,
CHECK_AND_DELETE,
CHECK_AND_READ,
CHECK_AND_UPDATE,
TEST_AND_SET_LOCK_OWNER,
RESET_LOCK_OWNER,
GET_STATES
};
struct NewMetaData {
    unsigned hyper instance_number;
    unsigned hyper content_generation_number;
    unsigned hyper lock_generation_number;
    unsigned hyper file_content_checksum;
    bool is_directory;
};
struct CheckAndCreateParam {
    string file_name<>;
    bool is_dir;
};
struct CheckAndCreateResult {
    unsigned instance_number;
    bool success;
};
struct CheckAndOpenParam {
    string file_name<>;
};
struct CheckAndOpenResult {
    unsigned instance_number;
    bool success;
};
struct CheckAndDeleteParam {
    string file_name<>;
    unsigned instance_number;
};
struct CheckAndDeleteResult {
    bool success;
};
struct CheckAndReadParam {
    string file_name<>;
    unsigned instance_number;
};
struct CheckAndReadResult {
    string content<>;
    NewMetaData meta;
    bool success;
};
struct CheckAndUpdateParam {
    string file_name<>;
    unsigned instance_number;
    string content<>;
};
struct CheckAndUpdateResult {
    bool success;
};
struct TestAndSetLockOwnerParam {
    string file_name<>;
    unsigned instance_number;
    string client_id<>;
};
struct TestAndSetLockOwnerResult {
    bool success;
};
struct ResetLockOwnerParam {
    string file_name<>;
    unsigned instance_number;
};
struct ResetLockOwnerResult {
    bool success;
};
struct GetStatesParam {
    bool dumb;
};
struct key_value {
  string key<>;
  string value<>;
};
struct GetStatesResult {
  key_value list<>;
};
union ServerPaxosParam switch (unsigned type) {
case CHECK_AND_CREATE:
    CheckAndCreateParam check_and_create_param;
case CHECK_AND_OPEN:
    CheckAndOpenParam check_and_open_param;
case CHECK_AND_DELETE:
    CheckAndDeleteParam check_and_delete_param;
case CHECK_AND_READ:
    CheckAndReadParam check_and_read_param;
case CHECK_AND_UPDATE:
    CheckAndUpdateParam check_and_update_param;
case TEST_AND_SET_LOCK_OWNER:
    TestAndSetLockOwnerParam check_and_test_lock_owner_param;
case RESET_LOCK_OWNER:
    ResetLockOwnerParam reset_lock_owner_param;
case GET_STATES:
    GetStatesParam get_states_param;
};
union ServerPaxosResult switch (unsigned type) {
case CHECK_AND_CREATE:
    CheckAndCreateResult check_and_create_result;
case CHECK_AND_OPEN:
    CheckAndOpenResult check_and_open_result;
case CHECK_AND_DELETE:
    CheckAndDeleteResult check_and_delete_result;
case CHECK_AND_READ:
    CheckAndReadResult check_and_read_result;
case CHECK_AND_UPDATE:
    CheckAndUpdateResult check_and_update_result;
case TEST_AND_SET_LOCK_OWNER:
    TestAndSetLockOwnerResult check_and_test_lock_owner_result;
case RESET_LOCK_OWNER:
    ResetLockOwnerResult reset_lock_owner_result;
case GET_STATES:
    GetStatesResult get_states_result;
};
