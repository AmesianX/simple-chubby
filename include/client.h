
#ifndef __CLIENT_H__
#define __CLIENT_H__


const uint64_t READ = 0x1;
const uint64_t WRITE = 0x2;
const uint64_t CREATE_DIRECTORY = 0x4;
const uint64_t CREATE_FILE = 0x8;

enum ErrorCode_t {
  NO_ERROR,
  BAD_ARG,
  FS_FAIL
};


class Client {
public:
    Client();
    ~Client();
    /*
     * Connect to a server
     */
    void open(const std::string &host);
    /*
     * Disconnect from a server
     */
    void close();
    /*
     * Test if still connected to a server
     *
     * Return Value: true if connected
     */
    bool isConnected();

    int increment(int input);
    int decrement(int input);
    void getEvent();

    /* Chubby APIs */
    FileHandler fileOpen(const std::string &file_name, Mode mode);
    void fileClose(const FileHandler &fd);
    bool fileDelete(const FileHandler &fd);
    bool getContentsAndStat(const FileHandler &fd,
			    FileContent *file_content, MetaData *meta_data);
    bool setContents(const FileHandler &fd, const FileContent &file_content);
    void acquire(const FileHandler &fd);
    bool tryAcquire(const FileHandler &fd);
    void release(const FileHandler &fd);

private:
    xdr::chubby_client_handler<api_v1> *client;
};

#endif /* __CLIENT_H__ */

