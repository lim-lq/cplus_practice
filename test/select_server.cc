#include <iostream>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <strings.h> // bzero
#include <string.h>  // memset
#include <string>
#include <errno.h>
#include <stdlib.h>
#include <pthread.h>
#include <vector>

using namespace std;

inline void print_err(const string& err)
{
    cout << err << " failure, error code[" << errno << "]." << endl;
}

inline void my_exit(const int& fd)
{
    close(fd);
    exit(1);
}

struct ClientInfo{
    ClientInfo(const string& h, const int& fd) : host(h), clientfd(fd){}
    string host;
    int clientfd;
};

pthread_mutex_t _lock;
vector<ClientInfo*> clientinfos;

void *do_select(void *arg)
{
    int maxfd = -1;
    fd_set readfds;
    char buf[1024];

    while ( true ) {
        timeval timeout = {3, 0};
        FD_ZERO(&readfds);
        pthread_mutex_lock(&_lock); 
        for ( vector<ClientInfo*>::iterator it = clientinfos.begin(); it != clientinfos.end(); ++it ) {
            // cout << "Add sock fd " << (*it)->clientfd << endl;
            FD_SET((*it)->clientfd, &readfds);
            maxfd = (*it)->clientfd > maxfd ? (*it)->clientfd : maxfd;
        }
        pthread_mutex_unlock(&_lock);
        maxfd += 1;
        cout << "Begin select wait for event" << endl;
        int ret = select(maxfd, &readfds, NULL, NULL, &timeout);

        if ( ret == 0 ) {
            // cout << "No even happen" << endl;
            continue;
        } else if ( ret == -1 ) {
            print_err("select event");
            break;
        }
        cout << ret << endl;

        for ( vector<ClientInfo*>::iterator it = clientinfos.begin(); it != clientinfos.end(); ++it ) {
            if ( FD_ISSET((*it)->clientfd, &readfds) ) {
                cout << "Some client send a message." << endl;
                memset(buf, 0, 1024);
                int size = recv((*it)->clientfd, buf, 1024, 0);
                ClientInfo* p_client_info;
                p_client_info = *it;
/*
                pthread_mutex_lock(&_lock);
                ClientInfo* p_client_info;
                vector<ClientInfo*>::iterator it;
                for ( it = clientinfos.begin(); it != clientinfos.end(); ++it ) {
                    if ( (*it)->clientfd == ret ) {
                        p_client_info = *it;
                        break;
                    }
                }
                pthread_mutex_unlock(&_lock);*/
                if ( size == 0 ) {
                    cout << "client shutdown" << endl;
                    clientinfos.erase(it);
                    delete p_client_info;
                } else if ( size == -1 ) {
                    print_err("recieve message");
                    continue;
                }

                cout << "Recieve message '" << buf << "' from client[" << p_client_info->host << "]." << endl;
            }            
        }
    }
}

int main(int argc, char* argv[])
{
    sockaddr_in serverAddr;
    int serverfd;

    string host = "10.192.0.5";
    unsigned short port = 0;

    serverfd = socket(AF_INET, SOCK_STREAM, 0);
    if ( serverfd == -1 ) {
        print_err("Create socket");
        my_exit(serverfd);
    }
    bzero(&serverAddr, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr(host.c_str());
    serverAddr.sin_port = htons(port);

    int on = 1;
    int ret = setsockopt(serverfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
    if ( ret == -1 ) {
        print_err("Set REUSEADDR");
        my_exit(1);
    }

    if ( bind(serverfd, (sockaddr*)&serverAddr, sizeof(serverAddr)) == -1 ) {
        print_err("Bind socket");
        my_exit(serverfd);
    }

    if ( listen(serverfd, 10) == -1 ) {
        print_err("Listen");
        my_exit(serverfd);
    }

    // create select thread
    pthread_t tid;
    if ( pthread_create(&tid, NULL, &do_select, NULL) != 0 ) {
        print_err("Create thread");
        my_exit(serverfd);
    }

    int clientfd;
    sockaddr_in clientAddr;
    socklen_t addrlen = sizeof(clientAddr);

    cout << "server running at " << host << ":" << port << endl;
    while ( true ) {
        bzero(&clientAddr, sizeof(clientAddr));
        clientfd = accept(serverfd, (sockaddr*)&clientAddr, &addrlen);
        if ( clientfd == -1 ) {
            print_err("Accept connection");
            my_exit(serverfd);
        }
        cout << "A new client[" << inet_ntoa(clientAddr.sin_addr) << "] connected." << endl;

/*        sleep(4);
        string message = "Hello world";
        int ret = send(clientfd, message.c_str(), message.size(), 0);
        if ( ret == -1 ) {
            print_err("Send message");
        }
        close(clientfd);*/
        pthread_mutex_lock(&_lock);
        clientinfos.push_back(new ClientInfo(inet_ntoa(clientAddr.sin_addr), clientfd));
        pthread_mutex_unlock(&_lock);
    }

    return 0;
}
