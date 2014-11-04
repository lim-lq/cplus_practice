/*
 * 2014-10-27
 * author: blond.li
 */
#include <iostream>
#include <sstream>

#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <strings.h>
#include <errno.h>

#include "log.hpp"
#include "utils.hpp"
#include "endpoint.h"

namespace wind
{
EndPoint::EndPoint(const int& fd) : m_serverfd(fd)
{
    sockaddr_in addr;
    socklen_t addrlen = sizeof(addr);

    int ret = ::getpeername(m_serverfd, (sockaddr*)&addr, &addrlen);
    if ( ret == -1 ) {
        LOG4CPLUS_ERROR(LOGGER, "Get client address failure, "
                                << " error code is [" << errno << "]");
        exit(1);
    }
    std::istringstream iss(::inet_ntoa(addr.sin_addr));
    iss >> m_host;
    m_port = ::ntohs(addr.sin_port);
}

EndPoint::EndPoint(const std::string& host, const uint16_t& port)
{
    m_serverfd = 0;
    m_serverfd = ::socket(AF_INET, SOCK_STREAM, 0);
    if ( m_serverfd == -1 ) {
        print_error("Create socket");
        exit(1);
    }
    int value = 1;
    ::setsockopt(m_serverfd, SOL_SOCKET, SO_REUSEADDR, &value, sizeof(value));

    m_host = host;
    m_port = port;
}

EndPoint::~EndPoint()
{
    if ( m_serverfd > 0 ) {
        LOG4CPLUS_INFO(LOGGER, "close the fd.");
        close(m_serverfd);
    }
}

int EndPoint::bind()
{
    sockaddr_in serverAddr;
    bzero(&serverAddr, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr(m_host.c_str());
    serverAddr.sin_port = htons(m_port);

    return ::bind(m_serverfd, (sockaddr*)&serverAddr, sizeof(serverAddr));
}

int EndPoint::listen(const uint32_t& listen_num)
{
    return ::listen(m_serverfd, listen_num);
}

int EndPoint::accept(sockaddr_in& clientAddr)
{
    socklen_t addrlen = sizeof(clientAddr);

    int clientfd = ::accept(m_serverfd, (sockaddr*)&clientAddr, &addrlen);
    return clientfd;
}

int EndPoint::connect()
{
    sockaddr_in serverAddr;
    bzero(&serverAddr, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr(m_host.c_str());
    serverAddr.sin_port = htons(m_port);

    return ::connect(m_serverfd, (sockaddr*)&serverAddr, sizeof(serverAddr));
}

int EndPoint::send(const int& clientfd, const std::string& message)
{
    return ::send(clientfd, message.c_str(), message.size(), 0);
}

int EndPoint::send(const std::string& message)
{
    return ::send(m_serverfd, message.c_str(), message.size(), 0);
}

int EndPoint::recv(const int& clientfd, char* buf, const uint32_t& size)
{
    return ::recv(clientfd, buf, size, 0);
}

int EndPoint::recv(char* buf, const uint32_t& size)
{
    return ::recv(m_serverfd, buf, size, 0);
}

} // end namespace wind