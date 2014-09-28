#include <sys/epoll.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <iostream>
#include <string>
#include <errno.h>
#include <stdlib.h>
#include <strings.h>

using namespace std;

inline void print_err(const string& err_msg)
{
    cout << err_msg << " failure, error code[" << errno << "]";
    if ( errno == EBADF ) {
        cout << " epfd or fd is not a valid file descriptor";
    }
    cout << "." << endl;
}


void set_nonblocking(const int& fd)
{
    int flags;

    flags = fcntl(fd, F_GETFL, 0);
    flags |= O_NONBLOCK;
    fcntl(fd, F_SETFL, flags);
}

int main(int argc, char* argv[])
{
    sockaddr_in serverAddr;
    sockaddr_in clientAddr;

    string host = "0.0.0.0";
    unsigned short port = 8000;

    int serverfd = socket(AF_INET, SOCK_STREAM, 0);
    // set_nonblocking(serverfd); // set socket fd mode to nonblocking

    if ( serverfd == -1 ) {
        print_err("Create socket");
        exit(1);
    }
    int on = 1;
    setsockopt(serverfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

    bzero(&serverAddr, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr(host.c_str());
    serverAddr.sin_port = htons(port);

    if ( bind(serverfd, (sockaddr*)&serverAddr, sizeof(serverAddr)) == -1 ) {
        print_err("Bind socket");
        exit(1);
    }

    listen(serverfd, 10);
    socklen_t addrlen = sizeof(clientAddr);
    cout << "server running at " << host << ":" << port << "." << endl;
    int clientfd = accept(serverfd, (sockaddr*)&clientAddr, &addrlen);
    if ( clientfd == -1 ) {
        print_err("Accept connect");
        close(serverfd);
        exit(1);
    }
    cout << "Connected by client[" << inet_ntoa(clientAddr.sin_addr) << "]." << endl;
    set_nonblocking(clientfd);
    // set epoll property
    int epollfd;
    int maxEvents = 64;
    epoll_event event;
    epoll_event* events;

    event.data.fd = clientfd;
    event.events = EPOLLIN | EPOLLET;
    epollfd = epoll_create(1);

    int ret = epoll_ctl(epollfd, EPOLL_CTL_ADD, clientfd, &event);
    if ( ret == -1 ) {
        print_err("Epoll control");
        close(clientfd);
        close(serverfd);
        close(epollfd);
        exit(1);
    }
    char buf[1024];
    events = (epoll_event*)calloc(maxEvents, sizeof(epoll_event));

    while ( true ) {
        int ret = epoll_wait(epollfd, events, maxEvents, -1);
        if ( ret == -1 ) {
            print_err("Epoll wait");
            close(clientfd);
            close(serverfd);
            close(epollfd);
            exit(1);
        } else if ( ret == 0 ) {
            cout << "No event" << endl;
            continue;
        }

        for ( int i = 0; i < ret; ++i ) {
            if ( events[i].events & EPOLLIN ) {
                bzero(buf, 1024);
                int ret = recv(events[i].data.fd, buf, 1024, 0);
                if ( ret == 0 ) {
                    cout << "Connect shutdown..." << endl;
                    close(events[i].data.fd);
                    epoll_ctl(epollfd, EPOLL_CTL_DEL, events[i].data.fd, NULL);
                    continue;
                } else if ( ret == -1 ) {
                    print_err("Recieve message");
                    continue;
                }
                cout << "Recieve message '" << buf << "'." << endl;
            }
        }
    }

    return 0;
}
