#ifndef __LOGINAPP_H__
#define __LOGINAPP_H__

#include <string>

#include <stdint.h>

#include "endpoint.h"
#include "threadPool.h"

namespace wind
{

struct ClientInfo
{
    ClientInfo(){}
    ClientInfo(const int& clientfd, const std::string& h, const uint16_t& p) : fd(clientfd), host(h), port(p)
    {
    }
    int fd;
    std::string host;
    uint16_t port;
};

typedef ClientInfo Task;

class WorkThread : public Thread
{
public:
    WorkThread(int id) : m_IsUsed(false), m_id(id)
    {
    }
    ~WorkThread()
    {
        std::cout << "Thread - " << m_id << " was been destroyed." << std::endl;
    }
    void run();
    void assign_task(const Task& task);
    bool isBusy()
    {
        return m_IsUsed;
    }
private:
    Task m_task;
    bool m_IsUsed;
    int m_id;
}; // end class WorkThread

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
    ThreadPool<WorkThread, Task> m_threadpool;
}; // end class LoginApp
} // end namespace wind

#endif