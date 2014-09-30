#include <sys/epoll.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <iostream>
#include <sstream>
#include <string>
#include <errno.h>
#include <stdlib.h>
#include <strings.h>

using namespace std;

inline void print_err(const string& msg_err)
{
    cout << msg_err << " failure, error code[" << errno << "]." << endl;
}

int main(int argc, char* argv[])
{
    sockaddr_in serverAddr;
    string host = "10.192.0.5";
    unsigned short port = 8000;
    istringstream iss;

    int opt = 0;
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

    int serverfd = socket(AF_INET, SOCK_STREAM, 0);
    if ( serverfd == -1 ) {
        print_err("Create socket");
        exit(1);
    }

    bzero(&serverAddr, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr(host.c_str());
    serverAddr.sin_port = htons(port);

    if ( connect(serverfd, (sockaddr*)&serverAddr, sizeof(serverAddr)) == -1 ) {
        print_err("Connect server");
        close(serverfd);
        exit(1);
    }

    cout << "connect server success" << endl;
    cout << " > ";
    string message;
    while ( true ) {
        getline(cin, message);
        int ret = send(serverfd, message.c_str(), message.size(), 0);
        if ( ret == -1 ) {
            print_err("Send message");
            close(serverfd);
            exit(1);
        }
        cout << "send message '" << message << "' success." << endl;
        cout << " > ";
    }

    return 0;
}