typedef string longstring<>;
typedef unsigned int ErrorCode;
typedef unsigned hyper Mode;

struct MetaData {
    unsigned hyper instance_number;
    unsigned hyper content_generation_number;
    unsigned hyper lock_generation_number;
    unsigned hyper file_content_checksum;
    bool is_directory;
};

typedef longstring FileContent;
typedef longstring DirectoryContent<>;

struct FileHandler {
    unsigned hyper magic_number;
    unsigned hyper master_sequence_number;
    longstring file_name;
    unsigned hyper instance_number;
    bool write_is_allowed;
};

struct ArgOpen {
    longstring name;
    Mode mode;
};

struct ArgSetContents {
    FileHandler fd;
    FileContent content;
};

struct ContentsAndStat {
    FileContent content;
    MetaData stat;
};

union RetBool switch (unsigned discriminant) {
case 0:
    bool val;
case 1:
    ErrorCode errCode;
default:
    void;
};


union RetFd switch (unsigned discriminant) {
case 0:
    FileHandler val;
case 1:
    ErrorCode errCode;
default:
    void;
};

union RetContentsAndStat switch (unsigned discriminant) {
case 0:
    ContentsAndStat val;
case 1:
    ErrorCode errCode;
default:
    void;
};


program server_api {
  version api_v1 {
	RetFd fileOpen(ArgOpen) = 1;
	RetBool fileClose(FileHandler) = 2;
	RetBool fileDelete(FileHandler) = 3;
	RetContentsAndStat getContentsAndStat(FileHandler) = 4;
	RetBool setContents(ArgSetContents) = 5;
	RetBool acquire(FileHandler) = 6;
	RetBool tryAcquire(FileHandler) = 7;
	RetBool release(FileHandler) = 8;

	int increment(int) = 991;
	int decrement(int) = 992;
  } = 1;
} = 0x40048086;
