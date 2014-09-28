#ifndef __SERVER_H__
#define __SERVER_H__

#include <iostream>
#include <errno.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string>
#include <vector>

namespace simplechat{

class ChatServer;
struct ConnectInfo{
    struct ClientInfo{
        ClientInfo(const int& fd, const struct sockaddr_in& addr) : clientfd(fd), clientAddr(addr)
        {}
        int clientfd;
        sockaddr_in clientAddr;       
    };
    ClientInfo* p_clientinfo;
    ChatServer* _server;
};

inline void strip(std::string& str)
{
    std::string::iterator it = str.begin();
    if ( *(it + str.size() - 1) == '\n' ) {
        str.erase(it + str.size() - 1);
    }
}

inline void print_err(const std::string& msg)
{
    std::string err_msg = "";
    if ( errno == EADDRINUSE ) { // code is 98
        err_msg = "Address is already in use.";
    }
    std::cout << msg << " failure, error code [ " << errno << " ]" << err_msg << std::endl;
}

class ChatServer{
public:
    ChatServer(const std::string& host, const unsigned short& port);
    ~ChatServer();
    void wait();
    void start_thread(const ConnectInfo* p_connectinfo);
    void do_connect(ConnectInfo::ClientInfo* p_clientinfo);
    void send_message(std::string& message, const std::string& clientIP);
private:
    static void* connect_proxy(void *ptr);

    std::string m_host;
    unsigned short m_port;
    int m_serverfd;
    bool m_RUNSTATUS;
    pthread_mutex_t m_lock;
    std::vector<ConnectInfo::ClientInfo*> m_clientInfos;
}; 

}

#endif //__SERVER_H__