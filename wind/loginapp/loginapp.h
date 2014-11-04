#ifndef __LOGINAPP_H__
#define __LOGINAPP_H__

#include <string>

#include <stdint.h>

#include "endpoint.h"
#include "threadpool.h"

namespace wind
{
class LoginTask : public Task
{
public:
    LoginTask(const int& fd, const std::string& dbmgrHost, const uint16_t& dbmgrPort);
    ~LoginTask();

    virtual void run();
private:
    EndPoint m_client;
    EndPoint m_dbmgr;
}; // end class LoginTask

class LoginApp
{
public:
    LoginApp(const std::string& host, const uint16_t& port);
    ~LoginApp(){}
    void run();
private:
    std::string m_host;
    uint16_t m_port;
    EndPoint m_endpoint;
    ThreadPool m_threadpool;
}; // end class LoginApp
} // end namespace wind

#endif