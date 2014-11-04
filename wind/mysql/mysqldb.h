#ifndef __MYSQLDB_H__
#define __MYSQLDB_H__

#include <stdint.h>
#include <mysql/mysql.h>

#include <iostream>
#include <string>
#include <stdexcept>
#include <vector>
#include <map>

namespace wind
{

typedef std::vector<std::map<std::string, std::string> > MYSQL_RESULT;

std::ostream& operator<<(std::ostream& os, const MYSQL_RESULT& result);

class MysqlDB{
public:
    MysqlDB();
    ~MysqlDB();
    ::MYSQL* connect(const std::string& host,
                   const std::string& user,
                   const std::string& passwd,
                   const std::string& db,
                   const uint32_t& port
                   );
    MYSQL_RESULT query(const std::string&);
    const char* error();
private:
    ::MYSQL m_connection;
    ::MYSQL_RES *m_result;
    ::MYSQL_ROW m_row;
    ::MYSQL_FIELD *m_fields;
}; // end MysqlDB
} // end namespace wind

#endif