#include "mysqldb.h"

namespace wind
{

MysqlDB::MysqlDB() : m_result(NULL), m_row(NULL), m_fields(NULL)
{
    if ( ::mysql_init(&m_connection) == NULL ) {
        throw std::runtime_error("Initialize mysql error");
    }
}

MysqlDB::~MysqlDB()
{
    ::mysql_close(&m_connection);
    ::mysql_free_result(m_result);
}

::MYSQL* MysqlDB::connect(const std::string& host,
                        const std::string& user,
                        const std::string& passwd,
                        const std::string& db,
                        const uint32_t& port)
{
    return ::mysql_real_connect(&m_connection,
                                host.c_str(),
                                user.c_str(),
                                passwd.c_str(),
                                db.c_str(),
                                port, 0, 0);
}

void MysqlDB::close()
{
    ::mysql_close(&m_connection);
    ::mysql_free_result(m_result);
}

MYSQL_RESULT MysqlDB::query(const std::string& sql)
{
    MYSQL_RESULT result;

    int ret = ::mysql_query(&m_connection, sql.c_str());
    if ( ret != 0 ) {
        return result;
    }
    m_result = ::mysql_store_result(&m_connection);
    // int row_num = ::mysql_num_rows(m_result);
    int filed_num = ::mysql_num_fields(m_result);
    m_fields = ::mysql_fetch_fields(m_result);

    while ( (m_row = ::mysql_fetch_row(m_result)) != NULL ) {
        std::map<std::string, std::string> line;
        for ( int i = 0; i < filed_num; ++i ) {
            line[m_fields[i].name] = m_row[i];
        }
        result.push_back(line);
    }
    return result;
}

const char* MysqlDB::error()
{
    return ::mysql_error(&m_connection);
}

std::ostream& operator<<(std::ostream& os, const MYSQL_RESULT& result)
{
    for ( MYSQL_RESULT::const_iterator it = result.begin(); it != result.end(); ++it ) {
        os << "--------------------------------------" << '\n';
        for ( std::map<std::string, std::string>::const_iterator mit = it->begin(); mit != it->end(); ++mit ) {
            os << mit->first << "\t" << mit->second << '\n';
        }
    }
    return os;
}

} // end namespace wind