#ifndef __THREADPOOL_H__
#define __THREADPOOL_H__

#include "task.hpp"

#include <pthread.h>

#include <list>
#include <tr1/memory>

namespace wind
{
class ThreadPool;
class Thread
{
public:
    Thread(ThreadPool* threadPool, const int& id);
    ~Thread();
    void LOCK();
    void UNLOCK();
    void wait();
    void signal();
    void create();
    void run();
    void setTask(Task* task);
private:
    static void* threadFunc(void *arg);
    pthread_cond_t m_cond;
    pthread_mutex_t m_lock;
    pthread_t m_tid;
    int m_id;
    Task* m_task;
    ThreadPool* m_threadPool;
}; // end class Thread

typedef std::tr1::shared_ptr<Thread> ThreadPtr;

class ThreadPool
{
public:
    ThreadPool();
    ~ThreadPool();
    void LOCK();
    void UNLOCK();
    void create(const int& num=10);
    int addTask(Task* task);
    void moveToFreeList(Thread* thread);
private:
    pthread_mutex_t m_lock;
    std::list<ThreadPtr> m_freeThreadList;
    std::list<ThreadPtr> m_busyThreadList;
}; // end class ThreadPool
} // end namespace wind

#endif