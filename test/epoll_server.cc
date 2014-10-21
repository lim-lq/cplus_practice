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
#include <pthread.h>
#include <strings.h>
#include <vector>

using namespace std;

vector<int> clientfds;
pthread_mutex_t lock;
pthread_mutex_init(&lock);

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

void *do_connect(void *arg)
{

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
/*    pthread_t tid;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);*/
    int ret = 0;

    if ( (ret = pthread_create(&tid, NULL, do_connect, NULL)) != 0 ) {
        cout << "Create pthread failure, error code[" << ret << ".\n";
        exit(1);
    }
    while ( true ) {
        int clientfd = accept(serverfd, (sockaddr*)&clientAddr, &addrlen);
        if ( clientfd == -1 ) {
            print_err("Accept connect");
            close(serverfd);
            exit(1);
        }
        cout << "Client " << inet_ntoa(clientAddr.sin_addr)
             << " connected" << endl;
        pthread_mutex_lock(&lock);
        clientfds.push_back(clientfd);
        pthread_mutex_unlock(&lock);
    }

    return 0;
}
