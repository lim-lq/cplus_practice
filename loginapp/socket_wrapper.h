#ifndef __SOCKET_WRAPPER_H__
#define __SOCKET_WRAPPER_H__

#include <iostream>
#include <string>
#include <errno.h>
#include <vector>
#include <tr1/memory>
#include <fcntl.h>
#include <map>

namespace coco
{

inline void print_err(std::string err_msg)
{
    std::cout << err_msg << " failure, error code[" << errno << "]." << std::endl; 
}

class Client
{
public:
    Client(const std::string& host, const int& clientfd) : m_host(host), m_clientfd(clientfd)
    {
        // set the client connection file description to nonblocking
        fcntl(clientfd, F_SETFD, fcntl(clientfd, F_GETFD, 0) | O_NONBLOCK);
    }
    ~Client()
    {
        close(m_clientfd);
    }
    std::string get_client_host()
    {
        return m_host;
    }
private:
    std::string m_host;
    int m_clientfd;
};
typedef std::tr1::shared_ptr<Client> ClientPtr;

class SocketWrapper
{
public:
    SocketWrapper(std::string& host, unsigned short port) : m_host(host), m_port(port)
    {
        m_serverfd = -1;
        m_runstatus = true;
    }
    ~SocketWrapper()
    {
        if ( m_serverfd > 0 ) {
            close(m_serverfd);
        }
    }

    int create_socket();
    static void *proxy(void* arg);
    void run();

protected:
    void do_connect();

private:
    std::string m_host;
    unsigned short m_port;
    int m_serverfd;
    int m_epollfd;
    bool m_runstatus;
    pthread_mutex_t m_lock;
    std::map<int, ClientPtr> m_clients;
};
} // end namespace coco

#endif