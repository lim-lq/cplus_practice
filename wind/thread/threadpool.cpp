#include "threadpool.h"
#include "log.hpp"

#include <iostream>
#include <sstream>
#include <stdexcept>

namespace wind
{
Thread::Thread(ThreadPool* threadPool, const int& id)
{
    pthread_mutex_init(&m_lock, NULL);
    pthread_cond_init(&m_cond, NULL);
    m_id = id;
    m_threadPool = threadPool;
}

Thread::~Thread()
{
    pthread_mutex_destroy(&m_lock);
    pthread_cond_destroy(&m_cond);
}

void Thread::LOCK()
{
    pthread_mutex_lock(&m_lock);
}

void Thread::UNLOCK()
{
    pthread_mutex_unlock(&m_lock);
}

void Thread::wait()
{
    LOCK();
    pthread_cond_wait(&m_cond, &m_lock);
    UNLOCK();
}

void Thread::signal()
{
    LOCK();
    pthread_cond_signal(&m_cond);
    UNLOCK();
}

void Thread::create()
{
    int ret = pthread_create(&m_tid, NULL, threadFunc, this);
    if ( ret != 0 ) {
        throw std::runtime_error("Create thread error.");
    }
}

void Thread::run()
{
    while ( true ) {
        std::cout << "I'm waiting for task, thread - " << m_id << "." << std::endl;
        wait();
        std::cout << "I'm the thread - " << m_id << "." << std::endl;
        LOG4CPLUS_INFO(LOGGER, "I'm the thread - " << m_id);
        m_task->run();
        if ( m_threadPool != NULL ) {
            m_threadPool->moveToFreeList(this);
        }
        delete m_task;
    }
}

void* Thread::threadFunc(void * arg)
{
    Thread* pth = (Thread*)arg;
    pth->run();
    return NULL;
}

void Thread::setTask(Task* task)
{
    m_task = task;
}

ThreadPool::ThreadPool()
{
    pthread_mutex_init(&m_lock, NULL);
}

ThreadPool::~ThreadPool()
{
    pthread_mutex_destroy(&m_lock);
}

void ThreadPool::LOCK()
{
    pthread_mutex_lock(&m_lock);
}

void ThreadPool::UNLOCK()
{
    pthread_mutex_unlock(&m_lock);
}

void ThreadPool::create(const int& num)
{
    for ( int i = 0; i < num; ++i ) {
        ThreadPtr thread(new Thread(this, i + 1));
        m_freeThreadList.push_back(thread);
    }
    for ( std::list<ThreadPtr>::iterator it = m_freeThreadList.begin(); it != m_freeThreadList.end(); ++it ) {
        (*it)->create();
    }
}

int ThreadPool::addTask(Task* task)
{
    if ( m_freeThreadList.empty() ) {
        return -1;
    }
    LOCK();
    ThreadPtr thread = m_freeThreadList.front();
    m_freeThreadList.pop_front();
    m_busyThreadList.push_back(thread);
    thread->setTask(task);
    thread->signal();
    UNLOCK();
    return 0;
}

void ThreadPool::moveToFreeList(Thread* thread)
{
    LOCK();
    for ( std::list<ThreadPtr>::iterator it = m_busyThreadList.begin(); it != m_busyThreadList.end(); ++it ) {
        if ( (*it).get() == thread ) {
            m_freeThreadList.push_back(*it);
            m_busyThreadList.erase(it);
            LOG4CPLUS_INFO(LOGGER, "Move thread to free list");
            break;
        }
    }
    UNLOCK();
}

} // end namespace wind