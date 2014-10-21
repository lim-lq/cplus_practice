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

namespace blond{

static void print_err(const std::string& msg)
{
    std::cout << msg << "failure, error code is " << errno << ".\n";
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
        if ( ret == -1 ) {
            sockaddr_in clientAddr;
            socklen_t addrlen = sizeof(clientAddr);
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
    }
}
} // end namespace blond