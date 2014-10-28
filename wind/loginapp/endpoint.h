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
    EndPoint();
    ~EndPoint();

    int bind(const std::string& host, const unsigned short& port);
    int accept(sockaddr_in& clientAddr);
    int listen(const uint32_t& listen_num);
    int send(const std::string& message);
    int recv(char* buf, const uint32_t size);
private:
    int m_serverfd;
    int m_clientfd;
    std::string m_host;
    unsigned short m_port;
};
} // end namspace wind

#endif