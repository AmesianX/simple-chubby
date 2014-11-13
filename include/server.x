
typedef string longstring<>;
typedef unsigned ErrorCode;
typedef longstring SetString<>;

union RPCBool switch (unsigned discriminant) {
case 0:
    bool val;
case 1:
    ErrorCode errCode;
default:
    void;
};

union RPCString switch (unsigned discriminant) {
case 0:
    string val<>;
case 1:
    ErrorCode errCode;
default:
    void;
};


union RPCSet switch (unsigned discriminant) {
case 0:
    SetString val;
case 1:
    ErrorCode errCode;
default:
    void;
};

struct kvpair {
	string key<512>;
	string val<>;
};

program server_api {
  version api_v1 {
	RPCBool create(kvpair) = 1;
	RPCBool remove(longstring) = 2;
	RPCString get(longstring) = 3;
	RPCBool set(kvpair) = 4;
	RPCSet list(longstring) = 5;
  } = 1;
} = 0x40048086;
