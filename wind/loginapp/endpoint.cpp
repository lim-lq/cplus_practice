/*
 * 2014-10-27
 * author: blond.li
 */
#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <stdlib.h>

#include "endpoint.h"

namespace wind
{
EndPoint::EndPoint(const std::string& host, const unsigned short& port)
{
    m_serverfd = socket(AF_INET, SOCK_STREAM, 0);
    if ( m_serverfd == -1 ) {
        print_error("Create socket");
        exit(1);
    }
}
} // end namespace wind