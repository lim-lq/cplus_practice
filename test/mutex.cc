#include "mutex.h"
#include <iostream>
#include <pthread.h>

namespace coco
{

void *MyMutex::do_thread(void * arg)
{
    std::cout << "I'm the thread process" << std::endl;
}

void MyMutex::create_thread()
{
    pthread_t tid;
    pthread_create(&tid, NULL, MyMutex::do_thread, NULL);
}

void MyMutex::run()
{
    create_thread();
    sleep(3);
    pthread_mutex_lock(&m_lock_lock);
    std::cout << "HAHAHA, I'm come in" << std::endl;
    pthread_mutex_unlock(&m_lock_lock);
}

}