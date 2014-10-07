#ifndef __MUTEX_H__
#define __MUTEX_H__

#include <sys/types.h>

namespace coco
{
class MyMutex
{
public:
    MyMutex(const int& numberID);
    ~MyMutex(){}
    void run();
    void create_thread();
private:
    int m_numberID;
    static void* do_thread(void *arg);
    pthread_mutex_t m_lock;
};
}

#endif