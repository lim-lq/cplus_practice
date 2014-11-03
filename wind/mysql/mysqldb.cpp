#include "mysqldb.h"

namespace wind
{

MysqlDB::MysqlDB()
{
    if ( mysql_init(m_connection) == NULL ) {
        throw std::runtime_error("Initialize mysql error");
    }
    m_fields = NULL;
    m_result = NULL;
    m_row = NULL;
}

MysqlDB::~MysqlDB()
{
    ::mysql_close(m_connection);
}

::MYSQL* MysqlDB::connect(const std::string& host,
                        const std::string& user,
                        const std::string& passwd,
                        const std::string& db,
                        const uint32_t& port)
{
    return ::mysql_real_connect(m_connection,
                                host.c_str(),
                                user.c_str(),
                                passwd.c_str(),
                                db.c_str(),
                                port, 0, 0);
}

MYSQL_RESULT query(const std::string& sql)
{
    MYSQL_RESULT result;

    int ret = ::mysql_query(m_connection, sql.c_str());
    if ( ret != 0 ) {
        return result;
    }
    m_result = ::mysql_store_result(m_connection);
    int row_num = ::mysql_num_rows(m_result);
    int filed_num = ::mysql_num_fileds(m_result);
    m_fields = ::mysql_fetch_fields(m_result);

    while ( (m_row = ::mysql_fetch_row(m_result)) != NULL ) {
        std::map<std::string, std::string> line;
        for ( int i = 0; i < filed_num; ++i ) {
            line[m_fields[i]] = m_row[i];
        }
        result.push_back(line);
    }
    return result;
}

} // end namespace wind