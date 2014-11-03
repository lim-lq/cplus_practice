#ifndef __THREADPOOL_H__
#define __THREADPOOL_H__

#include "task.hpp"

#include <pthread.h>

namespace wind
{
class Thread
{
public:
    Thread();
    ~Thread();
    int create();
    void run();
    void setTask(const Task& task);
private:
    static void* threadFunc(void *arg);
    pthread_cond_t m_cond;
    pthread_mutex_t m_lock;
    pthread_t m_tid;
    Task m_task;
}; // end class Thread
} // end namespace wind

#endif