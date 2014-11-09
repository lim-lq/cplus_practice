#include "log.hpp"
#include "cipher.h"
#include "loginapp.h"

#include <vector>
#include <iostream>
#include <sstream>
#include <fstream>
#include <stdexcept>
#include <tr1/memory>

#include <errno.h>
#include <stdint.h>
#include <string.h>
#include <pthread.h>
#include <sys/stat.h>

using namespace std;
using namespace wind;

struct LoginInfo
{
    LoginInfo(const std::string& host, const uint16_t& port, const int& id)
    : m_host(host), m_port(port), m_tid(id)
    {

    }
    string m_host;
    uint16_t m_port;
    int m_tid;
};

void* login(void* args)
{
    LoginInfo* p_loginInfo = (LoginInfo*)args;
    string host = p_loginInfo->m_host;
    uint16_t port = p_loginInfo->m_port;

    ostringstream oss;
    oss << "./" << p_loginInfo->m_tid;
    cout << oss.str() << endl;
    mkdir(oss.str().c_str(), 0744);
    string publicKey_dir = oss.str() + "/public.key";
    cout << publicKey_dir << endl;
    // string publicKey_dir = "public.key";

    EndPoint endpoint(host, port);
    int ret = endpoint.connect();

    if ( ret == -1 ) {
        LOG4CPLUS_INFO(LOGGER, "Connect server " << host
                            << ":" << port << " failure, error code is[" << errno << "]");
        return NULL;
    }

    uint8_t lenbuf[4];
    ret = endpoint.recv((char*)lenbuf, 4);
    if ( ret == -1 ) {
        LOG4CPLUS_INFO(LOGGER, "Recieve message from server "
                                << host << ":" << port
                                << " failure, error code is["
                                << errno << "]");
        return NULL;
    } else if ( ret == 0 ) {
        LOG4CPLUS_INFO(LOGGER, "Server " << host << ":" << port
                                << " has shutdown the connect.");
        return NULL;
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
            return NULL;
        } else if ( ret == 0 ) {
            LOG4CPLUS_INFO(LOGGER, "Server " << host << ":" << port
                                    << " has shutdown the connect.");
            return NULL;
        }

        memcpy(public_key.get() + recv_size, tmpbuf.get(), ret);
        recv_size += ret;
    }

    fstream file(publicKey_dir.c_str(), fstream::out | fstream::trunc);
    file << public_key.get();
    file.close();
    Rsa rsa;

    rsa.setPublicKey(publicKey_dir.c_str());
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
        return NULL;
    }

    cout << "Send " << ret << " size data." << endl;
    delete p_loginInfo;
    return NULL;
}

int main(int argc, char* argv[])
{
    string host = "192.168.200.128";
    uint16_t port = 8000;
    istringstream iss;
    int connect_num = 10;

    int opt = 0;
    while ( (opt = getopt(argc, argv, "i:p:n:h")) != -1 ) {
        switch ( opt ) {
            case 'i':
                host = optarg;
                break;
            case 'p':
                iss.str(optarg);
                iss >> port;
                break;
            case 'n':
                iss.str(optarg);
                iss >> connect_num;
                break;
            case 'h':
                cout << "Usage: "
                     << argv[0]
                     << " -i <host>"
                     << " -p <port>"
                     << " -n <connect num>."
                     << endl;
                return 0;
            default:
                break;
        }
    }

    wind::common::LoggerInitialize("client.log");

    vector<int> threads;

    int ret;

    for ( int i = 0; i < connect_num; ++i ) {
        pthread_t tid;
        LoginInfo* info = new LoginInfo(host, port, i);
        try {
            if ( (ret = pthread_create(&tid, NULL, login, info)) != 0 ) {
                throw std::invalid_argument("Create thread error");
            }
        } catch ( const std::invalid_argument& e ) {
            cout << e.what() << endl;
            cout << ret << endl;
            cout << "EAGAIN: " << EAGAIN << endl;
            cout << "EINVAL: " << EINVAL << endl;
            cout << "EPERM: " << EPERM << endl;
            return -1;
        }
        threads.push_back(tid);
    }

    for ( vector<int>::iterator it = threads.begin(); it != threads.end(); ++it ) {
        pthread_join(*it, NULL);
    }

    return 0;
}