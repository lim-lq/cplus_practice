/*
 * 2014-10-27
 * author: blond.li
 */

#ifndef __ENDPOINT_H__
#define __ENDPOINT_H__

#include <stdint.h>
#include <string>
#include <errno.h>
#include <netinet/in.h>

namespace wind
{

class EndPoint{
public:
    EndPoint(){}
    EndPoint(const int& fd);
    EndPoint(const std::string& host, const uint16_t& port);
    ~EndPoint();

    int bind();
    int accept(sockaddr_in& clientAddr);
    int listen(const uint32_t& listen_num);
    int send(const int& clientfd, const std::string& message);
    int send(const std::string& message);
    int recv(const int& clientfd, char* buf, const uint32_t& size);
    int recv(char* buf, const uint32_t& size);
    int connect();
    const std::string& getHost()
    {
        return m_host;
    }
    const uint16_t& getPort()
    {
        return m_port;
    }
    const int& getfd()
    {
        return m_serverfd;
    }
private:
    int m_serverfd;
    std::string m_host;
    unsigned short m_port;
};
} // end namspace wind

#endif