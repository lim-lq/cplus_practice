#ifndef __THREADPOO_H__
#define __THREADPOO_H__

#include <stdint.h>
#include <pthread.h>

#include <iostream>
#include <string>
#include <tr1/memory>
#include <vector>

namespace wind
{
class Thread
{
public:
    Thread()
    {
        pthread_mutex_init(&m_lock, NULL);
        pthread_cond_init(&m_cond, NULL);
    }
    ~Thread()
    {
        pthread_mutex_destroy(&m_lock);
        pthread_cond_destroy(&m_cond);
    }
    void start();
    virtual void run() = 0;
    void LOCK()
    {
        pthread_mutex_lock(&m_lock);
    }
    void UNLOCK()
    {
        pthread_mutex_unlock(&m_lock);
    }
    void wait()
    {
        pthread_cond_wait(&m_cond, &m_lock);
    }
    void signal()
    {
        pthread_cond_signal(&m_cond);
    }
    pthread_mutex_t& MUTEX()
    {
        return m_lock;
    }
private:
    static void *thread_func(void* arg);
    pthread_t m_tid;
    pthread_mutex_t m_lock;
    pthread_cond_t m_cond;
}; // end class Thread

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
    void assign_task(const std::string& task);
    bool isBusy()
    {
        return m_IsUsed;
    }
private:
    std::string m_task;
    bool m_IsUsed;
    int m_id;
}; // end class WorkThread

class ThreadPool
{
public:
    ThreadPool();
    ~ThreadPool();
    void create(const uint32_t thread_num = 10);
    int push_task(const std::string& task);
private:
    std::vector<std::tr1::shared_ptr<WorkThread> > m_threads;
}; // end class ThreadPool

} // end namespace wind
#endif