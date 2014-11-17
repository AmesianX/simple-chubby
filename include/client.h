
#ifndef __CLIENT_H__
#define __CLIENT_H__

#include <set>
#include <string>

enum ClientError {
    /*
     * Set, Get or Remove failed because they key was not found.
     */
    KEY_NOT_FOUND,
    /*
     * Create operation failed because the key has no parent.
     */
    NO_PARENT,
    /*
     * The key has children so it cannot be deleted.
     */
    HAS_CHILDREN,
    /*
     * The key path violates our formatting rules
     */
    MALFORMED_KEY,
};

/*
 * For server and/or library errors that must delivered to the client
 */
class ClientException : public std::exception {
public:
    ClientException(enum ClientError err) { errcode = err; }
    virtual ~ClientException() { }
    ClientError code() const { return errcode; }
    const char *what() {
        switch (errcode) {
            case KEY_NOT_FOUND:
                return "KEY NOT FOUND";
            case NO_PARENT:
                return "NO PARENT";
            case HAS_CHILDREN:
                return "HAS CHILDREN";
            case MALFORMED_KEY:
                return "MALFORMED KEY";
        }
        return "UNKNOWN ERROR CODE";
    }
private:
    enum ClientError errcode;
};

/*
 * For all the remote calls in the Client class you should use the ClientError 
 * class to throw exceptions that the client's can understand.
 */
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
    
    /* Chubby APIs */
    FileHandler fileOpen(const std::string &file_name, Mode mode);
    void fileClose(const FileHandler &fd);
    bool fileDelete(const FileHandler &fd);
    bool getContentsAndStat(const FileHandler &fd, FileContent *file_content, MetaData *meta_data);
    bool setContents(const FileHandler &fd, const FileContent &file_content);
    void acquire(const FileHandler &fd);
    bool tryAcquire(const FileHandler &fd);
    void release(const FileHandler &fd);

private:
    xdr::srpc_client<api_v1> *client;
};

#endif /* __CLIENT_H__ */

