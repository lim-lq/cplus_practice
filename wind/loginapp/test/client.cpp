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

    int data_length = *reinterpret_cast<int*>(lenbuf);

    cout << "Need recieve " << data_length << " byte data." << endl;
    int recv_size = 0;
    int once_size = data_length % 2048;

    std::tr1::shared_ptr<char> tmpbuf(new char[once_size]);
    std::tr1::shared_ptr<char> public_key(new char[data_length]);

    while ( recv_size != data_length ) {
        ret = endpoint.recv(tmpbuf.get(), once_size);
        cout << "Recieve " << ret << " bytes data" << endl;
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

        memcpy(public_key.get() + recv_size, tmpbuf.get(), ret);
        recv_size += ret;
    }

    fstream file("public.key", fstream::out | fstream::trunc);
    file << public_key.get();
    file.close();
    Rsa rsa;

    rsa.setPublicKey("public.key");
    // rsa.setPrivateKey("private.key");

    string data = "login\tliqing 123\0";

    // bytePtr cipher = rsa.publicEncrypt(data.c_str());
    // bytePtr plain = rsa.privateDecrypt(cipher);
    // cout << plain.first.get() << endl;
/*    ostringstream oss;
    oss << data.size() << '\t' << data;*/
    bytePtr cipher = rsa.publicEncrypt(data.c_str());
    file.open("cipher.txt", fstream::out | fstream::trunc);
    file << cipher.first.get();
    file.close();
    int plain_size = data.size();
    int cipher_size = rsa.getRsaSize();
    int data_size = cipher_size + 4 + 4;
    /*
     * 消息字段定义
     * method\tdata
     * login    user passwd
     * register user passwd
     */
    file.open("cipher.txt", fstream::out | fstream::trunc);
    file << cipher.first.get();
    file.close();

    // cipher size and plain size and cipher
    uint8_t msg[data_size];
    memcpy(msg, reinterpret_cast<void*>(&cipher_size), 4);
    memcpy(msg + 4, reinterpret_cast<void*>(&plain_size), 4);
    memcpy(msg + 4 + 4, cipher.first.get(), cipher_size);

    ret = endpoint.send(msg, sizeof(msg));

    if ( ret == -1 ) {
        LOG4CPLUS_INFO(LOGGER, "Send message to server "
                                << host << ":" << port
                                << " failure, error code is["
                                << errno << "]");
        return -1;
    }

    cout << "Send " << ret << " size data." << endl;
    return 0;
}