#include <iostream>
#include <pthread.h>
#include "mutex.h"

namespace coco
{
MyMutex::MyMutex(const int& numberID)
{
    m_numberID = numberID;
    pthread_mutex_init(&m_lock, NULL);
    std::cout << "call Construct function and set the numberID to "
              << m_numberID << "." << std::endl;
}

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
    pthread_mutex_lock(&m_lock);
    std::cout << "HAHAHA, I'm come in" << std::endl;
    pthread_mutex_unlock(&m_lock);
}

}