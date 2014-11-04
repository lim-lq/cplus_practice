#include "log.hpp"
#include "loginapp.h"

#include <iostream>
#include <sstream>

#include <errno.h>

using namespace std;
using namespace wind;

int main(int argc, char* argv[])
{
    string host = "192.168.200.128";
    uint16_t port = 8000;
    istringstream iss;

    int opt = 0;
    while ( (opt = getopt(argc, argv, "i:p:h")) != -1 ) {
        switch ( opt ) {
            case 'i':
                host = optarg;
                break;
            case 'p':
                iss.str(optarg);
                iss >> port;
                break;
            case 'h':
                cout << "Usage: "
                     << argv[0]
                     << " -i <host>"
                     << " -p <port>." << endl;
                return 0;
            default:
                break;
        }
    }

    wind::common::LoggerInitialize("client.log");

    EndPoint endpoint(host, port);
    int ret = endpoint.connect();

    if ( ret == -1 ) {
        LOG4CPLUS_INFO(LOGGER, "Connect server " << host
                            << ":" << port << " failure, error code is[" << errno << "]");
        return -1;
    }

    ret = endpoint.send("I'm a client");

    if ( ret == -1 ) {
        LOG4CPLUS_INFO(LOGGER, "Send message to server "
                                << ":" << port
                                << " failure, error code is["
                                << errno << "]");
        return -1;
    }

    return 0;
}