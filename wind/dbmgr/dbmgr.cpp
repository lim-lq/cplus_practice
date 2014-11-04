#include <stdexcept>
#include <string>

#include <errno.h>
#include <arpa/inet.h>

#include "dbmgr.h"
#include "log.hpp"
#include "configureParser.hpp"

namespace wind
{

DBmgrTask::DBmgrTask(const int& fd) : m_client(EndPoint(fd))
{

}

void DBmgrTask::run()
{
    ConfigureParser& config = SingletonConfigureParser::instance();
    
    std::string host;
    std::string user;
    std::string passwd;
    std::string db;
    int port;

    if ( config.get("mysql", "host").empty() ) {
        throw std::runtime_error("Configure file no mysql-host");
    }
    host = config.get("mysql", "host");

    if ( config.get("mysql", "user").empty() ) {
        throw std::runtime_error("Configure file no mysql-user");
    }
    user = config.get("mysql", "user");

    if ( config.get("mysql", "passwd").empty() ) {
        throw std::runtime_error("Configure file no mysql-passwd");
    }
    passwd = config.get("mysql", "passwd");

    if ( config.get("mysql", "db").empty() ) {
        throw std::runtime_error("Configure file no mysql-db");
    }
    db = config.get("mysql", "db");

    if ( config.get("mysql", "port").empty() ) {
        throw std::runtime_error("Configure file no mysql-port");
    }
    port = config.getInt("mysql", "port");

    if ( m_mysql.connect(host, user, passwd, db, port) == NULL ) {
        LOG4CPLUS_ERROR(LOGGER, m_mysql.error());
        return;
    }

    char buf[2048];
    int ret = m_client.recv(buf, 2048);
    if ( ret == -1 ) {
        LOG4CPLUS_ERROR(LOGGER, "Recieve message from client "
                                << m_client.getHost() << ":"
                                << m_client.getPort()
                                << " failure, error code is ["
                                << errno << "]");
        return;
    } else if ( ret == 0 ) {
        LOG4CPLUS_ERROR(LOGGER, "Client " << m_client.getHost()
                                << ":" << m_client.getPort()
                                << " has shutdown the connection");
        return;
    }

    LOG4CPLUS_INFO(LOGGER, "Recieve message: [" << buf << "]"
                            << " from client " << m_client.getHost()
                            << ":" << m_client.getPort());
    MYSQL_RESULT result = m_mysql.query("select * from users");
    std::cout << result << std::endl;
}

DBmgr::DBmgr(const std::string& host, const uint16_t& port) : m_endpoint(EndPoint(host, port))
{
    int ret = m_endpoint.bind();
    if ( ret == -1 ) {
        LOG4CPLUS_ERROR(LOGGER, "Bind socket failure, error code is["
                                << errno << "]");
        throw std::runtime_error("Bind socket failure");
    }

    ConfigureParser& config = SingletonConfigureParser::instance();

    int size;
    if ( config.get("dbmgr", "threadPoolSize").empty() ) {
        LOG4CPLUS_WARN(LOGGER, "There is no threadPoolSize setted, use default 100");
        size = 100;
    } else {
        size = config.getInt("dbmgr", "threadPoolSize");
    }

    m_threadpool.create(size);
}

void DBmgr::run()
{
    sockaddr_in clientAddr;
    LOG4CPLUS_INFO(LOGGER, "DBmgr server running at " << m_endpoint.getHost()
                            << ":" << m_endpoint.getPort());
    while ( true ) {
        int clientfd = m_endpoint.accept(clientAddr);
        if ( clientfd == -1 ) {
            LOG4CPLUS_ERROR(LOGGER, "Client "
                                    << "connected failure, error code is [" << errno << "]");
            continue;
        }

        DBmgrTask* task = new DBmgrTask(clientfd);
        m_threadpool.addTask(task);
    }
}

} // end namespace wind