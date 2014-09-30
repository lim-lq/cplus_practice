#ifndef __LOGINAPP_H__
#define __LOGINAPP_H__

#include <string>
#include <mysql/mysql.h>

namespace coco
{

class MysqlDB
{
public:
    MysqlDB(const std::string& host, const std::string& user, const std::string& passwd, 
            const std::string& db, const unsigned int& port=3306)
            : m_host(host), m_user(user), m_passwd(passwd), m_db(db), m_port(port)
    {}
    ~MysqlDB();
    int connect();
    int query(const std::string& sql);
    int count_result();
private:
    MYSQL m_connection;
    MYSQL_RES *m_result;
    MYSQL_ROW m_row;
    std::string m_host;
    std::string m_user;
    std::string m_passwd;
    std::string m_db;
    unsigned int m_port;
};

class LoginApp{
public:
    LoginApp(){
        m_mysqldb = NULL;
    }
    ~LoginApp(){}
    void Initmysql(MysqlDB* mysqldb);
    int login(const std::string& username, const std::string& passwd);
private:
    MysqlDB *m_mysqldb;
}; // end class LoginApp
} // end namespace coco
#endif