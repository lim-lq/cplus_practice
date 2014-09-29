#include "loginapp.h"
#include <iostream>

namespace coco{
MysqlDB::~MysqlDB()
{
    mysql_free_result(m_result);
    mysql_close(m_connection);
}

int MysqlDB::connect()
{
    if ( mysql_init(&m_connection) == NULL ) {
        std::cout << mysql_error(&m_connection) << std::endl;
        return -1;
    }

    MYSQL *connect;

    connect = mysql_real_connect(&m_connection,
                                 m_host.c_str(),
                                 m_user.c_str(),
                                 m_passwd.c_str(),
                                 m_db.c_str()
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

LoginApp::LoginApp()
{
    m_mysqldb = MysqlDB()
}

int LoginApp::login(const std::string& username, const std::string& passwd)
{

}
} // end namespace coco