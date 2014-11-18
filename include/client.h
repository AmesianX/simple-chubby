
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
private:
    xdr::srpc_client<test_version> *client;
};

#endif /* __CLIENT_H__ */

