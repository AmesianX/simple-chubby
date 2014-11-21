
#ifndef __CLIENT_H__
#define __CLIENT_H__
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
    bool getContentsAndStat(const FileHandler &fd, FileContent *file_content, MetaData *meta_data);
    bool setContents(const FileHandler &fd, const FileContent &file_content);
    void acquire(const FileHandler &fd);
    bool tryAcquire(const FileHandler &fd);
    void release(const FileHandler &fd);

private:
    xdr::chubby_client_handler<api_v1> *client;
};

#endif /* __CLIENT_H__ */

