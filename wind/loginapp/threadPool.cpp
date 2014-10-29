/*
 * 2014-10-29
 * author: blond.li
 */

#include <iostream>
#include <sstream>
#include <stdexcept>
#include "threadPool.h"

namespace wind
{
void* Thread::thread_func(void* arg)
{
    Thread* pthread = (Thread*)arg;
    pthread->run();
    return NULL;
}
void Thread::start()
{
    if ( pthread_create(&m_tid, NULL, thread_func, this) != 0 ) {
        throw std::runtime_error("Create thread error");
    }
}

void WorkThread::run()
{
    while ( true ) {
        LOCK();
        wait();
        UNLOCK();
        std::cout << "I'm the thread " << m_id << std::endl;
        std::cout << "And I get the task [" << m_task << "]." << std::endl;
        sleep(20);
        m_IsUsed = false;
    }
}

void WorkThread::assign_task(const std::string& task)
{
    LOCK();
    m_task = task;
    m_IsUsed = true;
    signal();
    UNLOCK();
}

ThreadPool::ThreadPool()
{

}

ThreadPool::~ThreadPool()
{

}

void ThreadPool::create(const uint32_t thread_num)
{
    for ( uint32_t i = 0; i < thread_num; ++i ) {
        std::tr1::shared_ptr<WorkThread> thread(new WorkThread(i));
        m_threads.push_back(thread);
    }
    for ( std::vector<std::tr1::shared_ptr<WorkThread> >::iterator it = m_threads.begin();
          it != m_threads.end(); ++it ) {
        (*it)->start();
    }
}

int ThreadPool::push_task(const std::string& task)
{
    int thread_size = m_threads.size();
    int i = 0;
    for ( ; i < thread_size; ++i ) {
        if ( m_threads[i]->isBusy() ) {
            continue;
        }
        m_threads[i]->assign_task(task);
        std::cout << "Assign task [" << task << "] to thread " << i << std::endl;
        // sleep(1);
        break;
    }
    if ( i >= thread_size ) {
        std::cout << "All threads are busy, please wait a minite to push task." << std::endl;
    }
    return 0;
}

} // end namespace wind