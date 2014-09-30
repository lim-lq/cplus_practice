#include "loginapp.h"
#include <iostream>
#include <stdlib.h>

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
} // end namespace coco