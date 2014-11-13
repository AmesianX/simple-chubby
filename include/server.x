
typedef string longstring<>;

struct kvpair {
	string key<512>;
	string val<>;
};

program server_api {
  version api_v1 {
	bool create(kvpair) = 1;
	bool remove(longstring) = 2;
	bool set(kvpair) = 3;
    // TODO: Fill me in
  } = 1;
} = 0x40048086;

