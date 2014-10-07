#ifndef __MYSQL_H__
#define __MYSQL_H__

#include <mysql/mysql.h>
#include <string>

namespace coco{
class MysqlDB{
public:
    MysqlDB(const std::string& host, const std::string& user, const std::string& passwd,
            const std::string& db, const unsigned int& port);
    void query(const std::string& sql);
    int count_result();
private:
    MYSQL m_mysql;
    MYSQL_RES *m_result;
}; // end MysqlDB
} // end namespace coco
#endif