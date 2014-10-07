#ifndef __DBMGR_H__
#define __DBMGR_H__

#include "mysql.h"

namespace coco{
class DBmgr{
public:
    DBmgr(const MysqlDB& db) : m_db(db){}
    bool check_account(const std::string& username);
    bool check_login(const std::string& username, const std::string& passwd);
private:
    MysqlDB m_db;
}; // end class DBmgr
} // end namespace coco

#endif