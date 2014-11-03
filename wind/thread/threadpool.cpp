#include "threadpool.h"

#include <iostream>

namespace wind
{
Thread::Thread()
{
    pthread_mutex_init(&m_lock);
    pthread_cond_init(&m_cond);
}

Thread::~Thread()
{
    pthread_mutex_destroy(&m_lock);
    pthread_cond_destroy(&m_cond);
}

void run()
{
    while ( true ) {
        sleep(2);
        std::cout << "I'm the thread process." << std::endl;
    }
}

void* Thread::threadFunc(void * arg)
{
    Thread* pth = (Thread*)arg;
    pth->run();
}

int create()
{
    return ::pthread_create(&m_tid, NULL, threadFunc, this);
}

} // end namespace wind