#include "loginapp.h"
#include "log.hpp"
#include "configureParser.hpp"

#include <string.h>
#include <errno.h>
#include <strings.h>
#include <arpa/inet.h>

#include <stdexcept>
#include <iostream>
#include <fstream>
#include <sstream>
#include <tr1/memory>

namespace wind
{
LoginTask::LoginTask(const int& fd, const std::string& dbmgrHost, const uint16_t& dbmgrPort) : 
m_client(EndPoint(fd)), m_dbmgr(EndPoint(dbmgrHost, dbmgrPort))
{
    m_rsa.setPrivateKey("private.key");
    std::fstream ifile("public.key", std::fstream::in);
    std::string line;
    std::ostringstream oss;
    while ( ifile.peek() != EOF ) {
        std::getline(ifile, line);
        oss << line << '\n';
    }

    m_publicKey = oss.str();
    LOG4CPLUS_INFO(LOGGER, "Public key is: \n"
                            << m_publicKey);    
    ifile.close();
}

LoginTask::~LoginTask()
{
    std::cout << "Deleted" << std::endl;
}

void LoginTask::run()
{
    char buf[2048];

    bzero(buf, 2048);
    int ret = -2;

    int32_t length = m_publicKey.size();
    uint8_t msg[4 + length];
    memcpy(msg, reinterpret_cast<void*>(&length), 4);
    memcpy(msg + 4, m_publicKey.c_str(), m_publicKey.size());
    std::cout << "HAHA" << *reinterpret_cast<uint32_t*>(msg) << "HAHA" << std::endl;
    ret = m_client.send((char*)msg);
    if ( ret == -1 ) {
        LOG4CPLUS_ERROR(LOGGER, "Send public key to client "
                                << m_client.getHost() << ":"
                                << m_client.getPort() << " failure,"
                                << " error code is [" << errno << "]");
        return;
    }

    while ( true ) {
        ret = m_client.recv(buf, 2048);
        if ( ret == 0 ) {
            LOG4CPLUS_INFO(LOGGER, "The client " << m_client.getHost()
                                    << ":" << m_client.getPort() << " has shutdown the connection");
            return;
        } else if ( ret == -1 ) {
            LOG4CPLUS_INFO(LOGGER, "Recieve message from client " << m_client.getfd() << " "
                                    << m_client.getHost() << ":" << m_client.getPort()
                                    << " failure, error code is [" << errno << "]");
            return;
        } else {
            break;
        }
    }

    bytePtr cipher(new uint8_t[256]);

    memcpy(cipher.get(), buf, 256);

    bytePtr plain = m_rsa.privateDecrypt(cipher);

    LOG4CPLUS_INFO(LOGGER, "Recieve message ["
                            << plain.get() << "] from client "
                            << m_client.getHost() << ":"
                            << m_client.getPort());

    ret = m_dbmgr.connect();
    if ( ret == -1 ) {
        LOG4CPLUS_ERROR(LOGGER, "Connect dbmgr server failure"
                                << ", error code is [" << errno << "] -- ");
        return;
    }

    ret = m_dbmgr.send(buf);
    if ( ret == -1 ) {
        LOG4CPLUS_ERROR(LOGGER, "Send message to dbmgr server failure"
                                << ", error code is [" << errno << "] -- ");
        return;            
    }

    LOG4CPLUS_INFO(LOGGER, "Send message " << buf << " to dbmgr"
                            << " " << m_dbmgr.getHost() << ":"
                            << m_dbmgr.getPort());

    ret = m_dbmgr.recv(buf, 2048);

    if ( ret == -1 ) {
        LOG4CPLUS_ERROR(LOGGER, "Recieve message from dbmgr server failure"
                                << ", error code is [" << errno << "] -- ");
        return;
    } else if ( ret == 0 ) {
        LOG4CPLUS_ERROR(LOGGER, "Dbmgr server has shutdown.");
        return;
    }

    LOG4CPLUS_INFO(LOGGER, "Recieve message [" << buf << "] from dbmgr server");

    ret = m_client.send(buf);
    if ( ret == -1 ) {
        LOG4CPLUS_ERROR(LOGGER, "Send to client " << m_client.getHost()
                                << ":" << m_client.getPort()
                                << "failure, error code is ["
                                << errno << "]");
        return;
    }

}

LoginApp::LoginApp(const std::string& host, const uint16_t& port) :
                   m_host(host), m_port(port), m_endpoint(EndPoint(host, port))
{
    // initialize socket
    int ret = m_endpoint.bind();
    if ( ret == -1 ) {
        LOG4CPLUS_ERROR(LOGGER, "Bind socket " << host << ":" << port
                                << " failure, error code is [" << errno << "]");
        throw std::runtime_error("Bind socket failure.");
    }
    ret = m_endpoint.listen(10);

    LOG4CPLUS_INFO(LOGGER, "Initialize loginapp success.");
    // initialize thread pool
    ConfigureParser& config = SingletonConfigureParser::instance();
    int size = 0;
    if ( config.get("loginapp", "threadPoolSize").empty() ) {
        LOG4CPLUS_ERROR(LOGGER, "There is no threadpool size setted, use default 100");
        size = 100;
    } else {
        size = config.getInt("loginapp", "threadPoolSize");
        if ( size < 0 ) {
            LOG4CPLUS_WARN(LOGGER, "There threadpool size set error, can't use negative number");
            throw std::runtime_error("There threadpool size set error, can't use negative number");
        }
    }
    m_threadpool.create(size);
}

void LoginApp::run()
{
    LOG4CPLUS_INFO(LOGGER, "Login App running.");
    std::cout << "Login App runing." << std::endl;

    ConfigureParser& config = SingletonConfigureParser::instance();

    std::string dbmgrHost;
    uint16_t dbmgrPort;

    if ( config.get("dbmgr", "host").empty() ) {
        throw std::invalid_argument("Configure file has no dbmgr-host");
    }
    dbmgrHost = config.get("dbmgr", "host");

    if ( config.get("dbmgr", "port").empty() ) {
        throw std::invalid_argument("Configure file has no dbmgr-port");
    }
    dbmgrPort = config.getInt16("dbmgr", "port");

    while ( true ) {
        sockaddr_in clientAddr;
        int clientfd = m_endpoint.accept(clientAddr);
        if ( clientfd == -1 ) {
            LOG4CPLUS_ERROR(LOGGER, "Accept connection failure, error code is [" << errno << "]");
            throw std::runtime_error("Accept connection failure");
        }
        LOG4CPLUS_INFO(LOGGER, "A new client connected -- " << ::inet_ntoa(clientAddr.sin_addr)
                                << ":" << ::ntohs(clientAddr.sin_port));
        std::cout << "Connect fd is " << clientfd << std::endl;
        LoginTask* task = new LoginTask(clientfd, dbmgrHost, dbmgrPort);
        m_threadpool.addTask(task);
    }
}

} // end namespace wind