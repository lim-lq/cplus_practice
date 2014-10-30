/*
 * 2014-10-27
 * author: blond.li
 */
#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <strings.h>

#include "log.hpp"
#include "utils.hpp"
#include "endpoint.h"

namespace wind
{

EndPoint::EndPoint()
{
    m_serverfd = 0;
    m_serverfd = ::socket(AF_INET, SOCK_STREAM, 0);
    if ( m_serverfd == -1 ) {
        print_error("Create socket");
        exit(1);
    }
}

EndPoint::~EndPoint()
{
    if ( m_serverfd > 0 ) {
        close(m_serverfd);
    }
}

int EndPoint::bind(const std::string& host, const unsigned short& port)
{
    m_host = host;
    m_port = port;
    sockaddr_in serverAddr;
    bzero(&serverAddr, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr(host.c_str());
    serverAddr.sin_port = htons(port);

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

int EndPoint::connect(const std::string& host, const uint16_t& port)
{
    sockaddr_in serverAddr;
    bzero(&serverAddr, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr(host.c_str());
    serverAddr.sin_port = htons(port);

    m_host = host;
    m_port = port;

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