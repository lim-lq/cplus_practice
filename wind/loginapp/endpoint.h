/*
 * 2014-10-27
 * author: blond.li
 */

#ifndef __ENDPOINT_H__
#define __ENDPOINT_H__

#include <iostream>
#include <stdint.h>
#include <string>

namespace wind{

inline void print_err(const std::string& msg)
{
    std::cout << msg << "failure, error code is[" << errno << "]." << endl;
}

class EndPoint{
public:
    EndPoint(const std::string& host, const unsigned short& port);
    ~EndPoint()
    {
        close(m_serverfd);
    }
    int listen(const uint32_t& listen_num);
    int send(const std::string& message);
    std::string recv();
private:
    int m_serverfd;
    std::string m_host;
    unsigned short m_port;
};
} // end namspace wind

#endif