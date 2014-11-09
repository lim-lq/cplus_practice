#ifndef __DBMGR_H__
#define __DBMGR_H__

#include "mysqldb.h"
#include "endpoint.h"
#include "threadpool.h"

#include <string>

#define ERROR_FORMAT    "error\tthe message format is wrong"
#define ERROR_LOGIN     "error\tLogin error"

namespace wind
{

class DBmgrTask : public Task
{
public:
    DBmgrTask(const int& fd);
    ~DBmgrTask();
    virtual void run();
private:
    EndPoint m_client;
    MysqlDB m_mysql;
}; // end class DBmgrTask

class DBmgr
{
public:
    DBmgr(const std::string& host, const uint16_t& port);
    ~DBmgr(){}
    void run();
private:
    EndPoint m_endpoint;
    ThreadPool m_threadpool;
}; // end class DBmgr
} // end namespace wind

#endif