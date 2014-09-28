#include <iostream>
#include <strings.h>
#include <errno.h>
#include <arpa/inet.h>
#include <strings.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <pthread.h>
#include <sstream>

using namespace std;

pthread_mutex_t lock;

inline void strip(std::string& str)
{
    std::string::iterator it = str.begin();
    if ( *(it + str.size() - 1) == '\n' ) {
        str.erase(it + str.size() - 1);
    }
}

inline void print_err(const std::string& msg)
{
    std::string err_msg = "";
    if ( errno == EADDRINUSE ) { // code is 98
        err_msg = "Address is already in use.";
    }
    std::cout << msg << " failure, error code [ " << errno << " ]" << err_msg << std::endl;
}

void *recv_msg(void *ptr)
{
    int *serverfd = (int *)ptr;
    char message[1024];

    while ( true ) {
        memset(message, 0, sizeof(message));
        int ret = recv(*serverfd, message, 1024, 0);
        if ( ret == 0 ) {
            cout << "Server has shutdown, and I'll exit" << std::endl;
            pthread_mutex_lock(&lock);
            close(*serverfd);
            pthread_mutex_unlock(&lock);
            exit(1);
        } else if ( ret == -1 ) {
            print_err("Recieve message from server");
            sleep(3);
            continue;
        }
        if ( strcmp(message, "[YOU]") == 0 ) {
            cout << "You > ";
            cout.flush();
            continue;
        }
        string msg = message;
        strip(msg);
        char buf[1024];
        memset(buf, 0, sizeof(buf));
        cout << buf << "\r" << msg << endl;
        cout << "You > ";
        cout.flush();
    }
}

int main(int argc, char* argv[])
{
    sockaddr_in serverAddr;
    int serverfd;
    string host = "192.168.200.128";
    unsigned short port = 8000;
    istringstream iss;

    int opt;
    while ( (opt = getopt(argc, argv, "i:p:")) != -1 ) {
        switch ( opt ) {
        case 'i':
            host = optarg;
            break;
        case 'p':
            iss.str(optarg);
            iss >> port;
            break;
        case 'h':
            cout << "Usage: " << argv[0]
                 << " -i host"
                 << " -p port"
                 << endl;
            return 0;
        default:
            break;
        }
    }

    serverfd = socket(AF_INET, SOCK_STREAM, 0);
    bzero(&serverAddr, sizeof(serverAddr));

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = inet_addr(host.c_str());

    if ( connect(serverfd, (sockaddr*)&serverAddr, sizeof(sockaddr)) == -1 ) {
        cout << "Connect server[ " << host << ":" << port << " failure, error code is " << errno << endl;
        exit(1);
    }

/*    cout << "Connect server success." << endl;

    fd_set readfds;
    fd_set writefds;
    int maxfd = -1;
    timeval timeout = {3, 0};
    char buf[1024];

    while ( true ) {
        FD_ZERO(&readfds);
        FD_SET(serverfd, &readfds);
        FD_SET(serverfd, &writefds);
        maxfd = serverfd + 1;

        int ret = select(maxfd, &readfds, &writefds, NULL, &timeout);
        if ( ret == 0 ) {
            continue;
        } else if ( ret == -1 ) {
            print_err("select event");
            close(serverfd);
            exit(1);
        } else {
            if ( FD_ISSET(serverfd, &writefds) ) {
                memset(buf, 0, sizeof(buf));
                int ret = recv(serverfd, buf, sizeof(buf), 0);
                if ( ret == 0 ) {
                    cout << "server shutdown the connect." << endl;
                    close(serverfd);
                    exit(1);
                } else if ( ret == -1 ) {
                    cout << "recieve message from server faliure, error code[" << errno << "]." << endl;
                    exit(1);
                }
                cout << "Recieve message " << buf << "." << endl;
            }
        }
    }
*/
    pthread_t tid;
    if ( pthread_create(&tid, NULL, recv_msg, &serverfd) == -1 ) {
        print_err("Create thread");
        close(serverfd);
        exit(1);
    }
    cout << "You connect socket fd is " << serverfd << endl;
    cout << "========================================\n"
         << "=======Welcome to Blond Chat Room=======\n"
         << "You > ";
    while ( true ) {
        string message;
        std::getline(cin, message);
        pthread_mutex_lock(&lock);
        int size = send(serverfd, message.c_str(), message.size(), 0);
        pthread_mutex_unlock(&lock);
        if ( size == -1 ) {
            print_err("Send message");
            break;
        }
    }
    close(serverfd);
    return 0;
}