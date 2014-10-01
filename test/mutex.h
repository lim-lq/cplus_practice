#ifndef __MUTEX_H__
#define __MUTEX_H__

#include <pthread.h>

namespace coco
{
class MyMutex
{
public:
    MyMutex(){}
    ~MyMutex(){}
    void run();
    void create_thread();
private:
    static void* do_thread(void *arg);
    pthread_mutex_t m_lock_lock;
};
}

#endif