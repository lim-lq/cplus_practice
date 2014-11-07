#include "log.hpp"
#include "cipher.h"
#include "loginapp.h"

#include <iostream>
#include <sstream>
#include <fstream>
#include <tr1/memory>

#include <errno.h>
#include <string.h>

using namespace std;
using namespace wind;

int main(int argc, char* argv[])
{
    string host = "10.192.0.5";
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

    uint8_t lenbuf[4];
    ret = endpoint.recv((char*)lenbuf, 4);
    if ( ret == -1 ) {
        LOG4CPLUS_INFO(LOGGER, "Recieve message from server "
                                << host << ":" << port
                                << " failure, error code is["
                                << errno << "]");
        return -1;
    } else if ( ret == 0 ) {
        LOG4CPLUS_INFO(LOGGER, "Server " << host << ":" << port
                                << " has shutdown the connect.");
        return -1;
    }

    const int data_length = *reinterpret_cast<int*>(lenbuf);

    cout << data_length << endl;
    int length = data_length;
    int recv_size = 0;
    const int once_size = data_length % 2048;

    std::tr1::shared_ptr<char> tmpbuf(new char[once_size]);
    // char public_key[length];

    cout << once_size << endl;

/*    while ( recv_size != data_length ) {
        ret = endpoint.recv(tmpbuf, data_length % 2048);
        cout << ret << endl;
        if ( ret == -1 ) {
            LOG4CPLUS_INFO(LOGGER, "Recieve message from server "
                                    << host << ":" << port
                                    << " failure, error code is["
                                    << errno << "]");
            return -1;
        } else if ( ret == 0 ) {
            LOG4CPLUS_INFO(LOGGER, "Server " << host << ":" << port
                                    << " has shutdown the connect.");
            return -1;
        }

        memcpy(public_key + recv_size, tmpbuf, ret);
        recv_size += ret;
    }

    fstream file("public.key", fstream::out | fstream::trunc);
    file << data_length;
    file.close();
    Rsa rsa;*/

    // rsa.setPublicKey("public.key");
    // bytePtr cipher = rsa.publicEncrypt("liqing 123");
    /*
     * 消息字段定义
     * method\tdata
     * login    user passwd
     * register user passwd
     */
/*    ostringstream oss;
    oss << "login\t" << cipher.get();
    ret = endpoint.send(oss.str());

    if ( ret == -1 ) {
        LOG4CPLUS_INFO(LOGGER, "Send message to server "
                                << host << ":" << port
                                << " failure, error code is["
                                << errno << "]");
        return -1;
    }*/

    return 0;
}