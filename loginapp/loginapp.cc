#include "loginapp.h"
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <vector>
#include <map>
#include <sys/socket.h>
#include <boost/algorithm/string.hpp>

namespace coco
{
MysqlDB::~MysqlDB()
{
    mysql_free_result(m_result);
    mysql_close(&m_connection);
}

int MysqlDB::connect()
{
    if ( mysql_init(&m_connection) == NULL ) {
        std::cout << mysql_error(&m_connection) << std::endl;
        return -1;
    }

    MYSQL *connect;
    std::cout << "Connect mysql(host=" << m_host
              << ", username=" << m_user
              << ", passwd=" << m_passwd
              << ", db="<< m_db
              << ", port=" << m_port << ")" << std::endl; 
    connect = mysql_real_connect(&m_connection,
                                 m_host.c_str(),
                                 m_user.c_str(),
                                 m_passwd.c_str(),
                                 m_db.c_str(),
                                 m_port, 0, 0);
    if ( connect == NULL ) {
        std::cout << mysql_error(&m_connection) << std::endl;
        return -1;
    }
    return 0;
}

int MysqlDB::query(const std::string& sql)
{
    int ret = mysql_query(&m_connection, sql.c_str());

    if ( ret != 0 ) {
        std::cout << mysql_error(&m_connection) << std::endl;
        return -1;
    }
    m_result = mysql_store_result(&m_connection);
    return 0;
}

int MysqlDB::count_result()
{
    return mysql_num_rows(m_result);
}

void LoginApp::Initmysql(MysqlDB* mysqldb)
{
    m_mysqldb = mysqldb;
    if ( m_mysqldb->connect() == -1 ) {
        exit(1);
    }
}

void LoginApp::start_server(const std::string& srvHost, const unsigned short& srvPort)
{
    int ret = create_socket(srvHost, srvPort);
    if ( ret == -1 ) {
        exit(1);
    }

    run();
}

int LoginApp::login(const std::string& username, const std::string& passwd)
{
    std::string sql = "select * from users where username='" +
                      username + "' and passwd='" + passwd + "'";
    std::cout << "Login " << username << ":" << passwd << std::endl;
    int ret = m_mysqldb->query(sql);
    if ( ret == -1 ) {
        return -1;
    }
    if ( m_mysqldb->count_result() == 0 ) {
        return -1;
    }
    return 0;
}

int LoginApp::addAccount(const std::string& username, const std::string& passwd)
{
    std::string sql = "insert into users(username, passwd) values('" +
                      username + "', '" + passwd + "')";
    std::cout << "Add account " << username << ":" << passwd << std::endl;
    int ret = m_mysqldb->query(sql);
    if ( ret == -1 ) {
        return -1;
    }
    return 0;
}

void LoginApp::deal_request(const std::string& msg, const int& clientfd)
{
    std::vector<std::string> messages;
    boost::algorithm::split(messages, msg, boost::algorithm::is_any_of(","));
    std::cout << "Recieve message: \n";
    for ( std::vector<std::string>::iterator it = messages.begin(); it != messages.end(); ++it ) {
        std::cout << *it << std::endl;
    }
    std::map<std::string, std::string> loginInfo;
    if ( messages.size() != 3 ) {
        std::string msg = "Login failure, field is error.";
        send(clientfd, msg.c_str(), msg.size(), 0);
        std::cout << msg << std::endl;
        return;
    }

    std::string info = "";
    if ( messages[0] == "login" ) {
        if ( login(messages[1], messages[2]) == -1 ) {
            info = "Login failure, please check your username or password.";
        } else {
            info = "Login success.";
        }
    } else if ( messages[0] == "addAccount" ) {
        if ( addAccount(messages[1], messages[2]) == -1 ) {
            info = "Add account failure, something wrong.";
        } else {
            info = "Add account success.";
        }
    } else {
        info = "Unkonwn operation '" + messages[0] + "'.";
    }
    send(clientfd, info.c_str(), info.size(), 0);
    std::cout << info << std::endl;
}
} // end namespace coco