#include "loginapp.h"
#include "log.hpp"

#include <string.h>
#include <errno.h>
#include <strings.h>
#include <arpa/inet.h>

#include <stdexcept>
#include <iostream>

namespace wind
{
std::string WorkThread::m_dbmgrHost = "";
uint16_t WorkThread::m_dbmgrPort = 0;

void WorkThread::setDBmgr(const std::string& host, const uint16_t& port)
{
    m_dbmgrHost = host;
    m_dbmgrPort = port;
}

WorkThread::WorkThread(const int& id) :
            m_dbmgr(EndPoint(WorkThread::m_dbmgrHost, WorkThread::m_dbmgrPort))
{
    m_IsUsed = false;
    m_IsExit = false;
    m_id = id;
}

WorkThread::~WorkThread()
{
    std::cout << "Thread - " << m_id << " was been destroyed." << std::endl;
}

void WorkThread::run()
{
    while ( true ) {
        LOCK();
        wait();
        UNLOCK();
        std::cout << "I'm the thread " << m_id << std::endl;
        std::cout << "And I get the task [" << m_task.fd << " "
                  << m_task.host << " " << m_task.port << "]." << std::endl;
        char buf[2048];
        while ( true ) {
            bzero(buf, 2048);
            int ret = ::recv(m_task.fd, buf, 2048, 0);
            if ( ret == 0 ) {
                LOG4CPLUS_INFO(LOGGER, "The client " << m_task.host
                                        << ":" << m_task.port << " has shutdown the connection");
                close(m_task.fd);
                bzero(&m_task, sizeof(m_task));
                break;
            } else if ( ret == -1 ) {
                LOG4CPLUS_INFO(LOGGER, "Recieve message from client "
                                        << m_task.host << ":" << m_task.port
                                        << " error.");
                close(m_task.fd);
                bzero(&m_task, sizeof(m_task));
                break;
            }

            LOG4CPLUS_INFO(LOGGER, "Recieve message ["
                                    << buf << "] from client "
                                    << m_task.host << ":"
                                    << m_task.port);
        }
        int ret = m_dbmgr.connect();
        if ( ret == -1 ) {
            LOG4CPLUS_ERROR(LOGGER, "Connect dbmgr server failure"
                                    << ", error code is [" << errno << "] -- "
                                    << " Worker-" << m_id);
            m_IsExit = true;
            return;
        }

        ret = m_dbmgr.send(buf);
        if ( ret == -1 ) {
            LOG4CPLUS_ERROR(LOGGER, "Send message to dbmgr server failure"
                                    << ", error code is [" << errno << "] -- "
                                    << " Worker-" << m_id);
            m_IsExit = true;
            return;            
        }

        ret = m_dbmgr.recv(buf, 2048);

        if ( ret == -1 ) {
            LOG4CPLUS_ERROR(LOGGER, "Recieve message from dbmgr server failure"
                                    << ", error code is [" << errno << "] -- "
                                    << " Worker-" << m_id);
            m_IsExit = true;
            return;
        } else if ( ret == 0 ) {
            LOG4CPLUS_ERROR(LOGGER, "Dbmgr server has shutdown.");
            m_IsExit = true;
            return;
        }

        ret = ::send(m_task.fd, buf, strlen(buf), 0);
        if ( ret == -1 ) {
            LOG4CPLUS_ERROR(LOGGER, "Send to client " << m_task.host
                                    << ":" << m_task.port
                                    << "failure, error code is ["
                                    << errno << "]");
            m_IsExit = true;
            return;
        }

        m_IsUsed = false;
    }
}

void WorkThread::assign_task(const Task& task)
{
    LOCK();
    m_task = task;
    m_IsUsed = true;
    signal();
    UNLOCK();
}

LoginApp::LoginApp(const std::string& host, const uint16_t& port) :
                   m_host(host), m_port(port), m_endpoint(EndPoint(host, port))
{
    // initialize socket
    int ret = m_endpoint.bind();
    if ( ret == -1 ) {
        LOG4CPLUS_ERROR(LOGGER, "Bind socket " << host << ":" << port
                                << " failure, error code is [" << errno << "]");
        throw std::runtime_error("Bind socket failure.");
    }
    ret = m_endpoint.listen(10);

    LOG4CPLUS_INFO(LOGGER, "Initialize loginapp success.");
    // initialize thread pool
    m_threadpool.create(100);
    m_threadpool.start();
}

void LoginApp::run()
{
    LOG4CPLUS_INFO(LOGGER, "Login App running.");
    std::cout << "Login App runing." << std::endl;
    while ( true ) {
        sockaddr_in clientAddr;
        int clientfd = m_endpoint.accept(clientAddr);
        if ( clientfd == -1 ) {
            LOG4CPLUS_ERROR(LOGGER, "Accept connection failure, error code is [" << errno << "]");
            throw std::runtime_error("Accept connection failure");
        }
        LOG4CPLUS_INFO(LOGGER, "A new client connected -- " << ::inet_ntoa(clientAddr.sin_addr)
                                << ":" << ::ntohs(clientAddr.sin_port));
        
        Task task(clientfd, ::inet_ntoa(clientAddr.sin_addr), ::ntohs(clientAddr.sin_port));
        m_threadpool.push_task(task);
    }
}

} // end namespace wind