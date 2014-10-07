#include "dbmgr.h"

namespace coco{
bool DBmgr::check_account(const std::string& username)
{
    std::string sql = "select * from users where username='" + username + "'";
    m_db.query(sql);
    if ( m_db.count_result() ) {
        return true;
    }
    return false;
}

bool DBmgr::check_login(const std::string& username, const std::string& passwd)
{
    std::string sql = "select * from users where username='" +
                      username + "' and passwd='" + passwd + "'";
    m_db.query(sql);
    if ( m_db.count_result() ) {
        return true;
    }
    return false;
}
} // end namespace coco