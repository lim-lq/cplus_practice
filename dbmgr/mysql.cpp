#include "mysql.h"
#include <iostream>
#include <stdlib.h>

namespace coco{
MysqlDB::MysqlDB(const std::string& host, const std::string& user, const std::string& passwd,
            const std::string& db, const unsigned int& port)
{
    if ( mysql_init(&m_mysql) == NULL ) {
        std::cout << mysql_error(&m_mysql) << std::endl;
        exit(1);
    }

    if ( mysql_real_connect(&m_mysql, host.c_str(), user.c_str(),
        passwd.c_str(), db.c_str(), port, 0, 0) == NULL ) {
        std::cout << "Connect mysql server error" 
                  << " " << host << std::endl;
        std::cout << mysql_error(&m_mysql) << std::endl;
        exit(1);
    }
    m_result = NULL;
}
void MysqlDB::query(const std::string& sql)
{
    if ( m_result != NULL ) {
        mysql_free_result(m_result);
        m_result = NULL;
    }
    if ( mysql_query(&m_mysql, sql.c_str()) != 0 ) {
        std::cout << mysql_error(&m_mysql) << std::endl;
        exit(1);
    }
    m_result = mysql_store_result(&m_mysql);
    if ( m_result == NULL ) {
        std::cout << mysql_error(&m_mysql) << std::endl;
        exit(1);
    }
}

int MysqlDB::count_result()
{
    return mysql_num_rows(m_result);
}
} // end namespace coco