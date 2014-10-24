/*
 * author: blond.li
 # 2014-10-221 23:59:34
 */

#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include "configure_wrapper.h"
#include <strings.h>

namespace blond{

static void print_err(const std::string& msg)
{
    std::cout << msg << "failure, error code is " << errno << ".\n";
}

void Connector::run()
{
    std::string host = "0.0.0.0";
    unsigned short port = 10001;

    int serverfd;
    sockaddr_in serverAddr;

    bzero(&serverAddr, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr(host.c_str());
    serverAddr.sin_port = htons(port);

    serverfd = socket(AF_INET, SOCK_STREAM, 0);

    int val = 1;
    setsockopt(serverfd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val));

    if ( serverfd == -1 ) {
        print_err("Create socket");
        return;
    }
    bind(serverfd, (sockaddr*)&serverAddr, sizeof(serverAddr));
    listen(serverfd, 10);

    std::cout << "Socket server thread beging runing at " << host << ":" << port << ".\n";
    while ( true ) {
        sockaddr_in clientAddr;
        socklen_t addrlen = sizeof(clientAddr);
        int clientfd;
        clientfd = accept(serverfd, (sockaddr*)&clientAddr, &addrlen);
        if ( clientfd == -1 ) {
            print_err("Accept connection");
            continue;
        }
        m_mutexPtr->lock();
        m_clientfdsPtr->push_back(clientfd);
        m_mutexPtr->unlock();
        std::cout << "A new client " << inet_ntoa(clientAddr.sin_addr) << " connected." << std::endl;
    }
}

std::string ConfigureWrapper::getkey(const Ice::Current&)
{
    return m_key;
}

Ice::Int ConfigureWrapper::setkey(const std::string& key, const Ice::Current&)
{
    m_key = key;
    send_command("update_key");
    return 0;
}

void ConfigureWrapper::send_command(const std::string& key)
{
    for ( std::vector<int>::iterator it = m_clientfds.begin();
          it != m_clientfds.end(); ++it ) {
        int ret = send(*it, key.c_str(), key.size(), 0);
        sockaddr_in clientAddr;
        socklen_t addrlen = sizeof(clientAddr);
        if ( ret == -1 ) {
            if ( getpeername(*it, (sockaddr*)&clientAddr, &addrlen) == -1 ) {
                std::cout << "Get peer name failure, socket file descriptor is "
                          << *it << " , error code is " << errno << ".\n";
                m_mutex.lock();
                m_clientfds.erase(it);
                m_mutex.unlock();
                continue;
            }
            std::cout << "Send command to client " << inet_ntoa(clientAddr.sin_addr)
                      << " failure, error code is [" << errno << "]." << std::endl;
            m_mutex.lock();
            m_clientfds.erase(it);
            m_mutex.unlock();
        }
        getpeername(*it, (sockaddr*)&clientAddr, &addrlen);
        std::cout << "Send command " << key << " to client " << inet_ntoa(clientAddr.sin_addr) << "." << std::endl;
    }
}

void ConfigureWrapper::startConnectThread()
{
    m_connector = std::tr1::shared_ptr<Connector>(new Connector(&m_clientfds, &m_mutex));
    m_connector->start();
}

} // end namespace blond