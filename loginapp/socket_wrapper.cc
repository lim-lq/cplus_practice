#include "socket_wrapper.h"
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <stdlib.h>

namespace coco
{
int SocketWrapper::create_socket()
{
    sockaddr_in serverAddr;
    m_serverfd = socket(AF_INET, SOCK_STREAM, 0);
    if ( m_serverfd == -1 ) {
        print_err("Create socket");
        return -1;
    }

    // fcntl(m_serverfd, F_SETFD, fcntl(m_serverfd, F_GETFD, 0) | O_NONBLOCK);
    int on = 1;
    setsockopt(m_serverfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
    bzero(&serverAddr, sizeof(serverAddr));

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr(m_host.c_str());
    serverAddr.sin_port = htons(m_port);

    if ( bind(m_serverfd, (sockaddr*)&serverAddr, sizeof(serverAddr)) == -1 ) {
        print_err("Bind socket");
        return -1;
    }

    listen(m_serverfd, 10);
    return 0;
}

void SocketWrapper::do_connect()
{
    m_epollfd = epoll_create(1);
    if ( m_epollfd == -1 ) {
        print_err("Create epoll");
        exit(1);
    }

    char buf[1024];
    int maxEvents = 64;
    epoll_event event;
    std::tr1::shared_ptr<epoll_event> events((epoll_event*)calloc(maxEvents, sizeof(epoll_event)));

    while ( true ) {
        pthread_mutex_lock(&m_lock);
        if ( m_clients.empty() ) {
            continue;
        }
        for ( std::map<int, ClientPtr>::iterator it = m_clients.begin(); it != m_clients.end(); ++it ) {
            event.data.fd = it->first;
            event.events = EPOLLIN | EPOLLET;
            if ( epoll_ctl(m_epollfd, EPOLL_CTL_ADD, it->first, &event) != 0 ) {
                print_err("Epoll_ctl client " + it->first);    
            }
        }
        pthread_mutex_unlock(&m_lock);

        int ret = epoll_wait(m_epollfd, events.get(), maxEvents, 3000);
        if ( ret == 0 ) {
            continue;
        } else if ( ret == -1 ) {
            print_err("epoll_wait");
            close(m_epollfd);
            m_epollfd = epoll_create(1);
            if ( m_epollfd == -1 ) {
                print_err("Create epoll");
                exit(1);
            }
        }

        for ( int i = 0; i < ret; ++i ) {
            if ( events.get()[i].events & EPOLLIN ) {
                bzero(buf, sizeof(buf));
                int ret = recv(events.get()[i].data.fd, buf, sizeof(buf), 0);
                if ( ret == 0 ) {
                    std::cout << "Client[" << m_clients[events.get()[i].data.fd]->get_client_host()
                              << "] shutdown." << std::endl;
                    m_clients.erase(events.get()[i].data.fd);
                    continue;
                } else if ( ret == -1 ) {
                    std::cout << "Recieve message from client["
                              << m_clients[events.get()[i].data.fd]->get_client_host()
                              << "] failure, error code[" << errno << "]." << std::endl;
                    continue;
                }
                std::cout << "Recieve message(" << buf << ") from client["
                          << m_clients[events.get()[i].data.fd]->get_client_host()
                          << "]." << std::endl;
            }
        }
    }
}

void* SocketWrapper::proxy(void* arg)
{
    SocketWrapper* p_socketWrapper = (SocketWrapper*)arg;
    p_socketWrapper->do_connect();
}

void SocketWrapper::run()
{
    std::cout << "Server is running at " << m_host << ":" << m_port << std::endl;

    pthread_t tid;
    if ( pthread_create(&tid, NULL, &proxy, this) != 0 ) {
        print_err("Create epoll thread");
        close(m_serverfd);
        return;
    }

    while ( m_runstatus ) {
        sockaddr_in clientAddr;
        socklen_t addrlen = sizeof(clientAddr);
        int clientfd = accept(m_serverfd, (sockaddr*)&clientAddr, &addrlen);
        if ( clientfd == -1 ) {
            print_err("Accept Connect");
            continue;
        }
        
        pthread_mutex_lock(&m_lock);
        m_clients.insert(std::make_pair(clientfd, new Client(inet_ntoa(clientAddr.sin_addr), clientfd)));
        pthread_mutex_unlock(&m_lock);

        std::cout << "Client [" << inet_ntoa(clientAddr.sin_addr) << "] connected!." << std::endl;
    }
}
}