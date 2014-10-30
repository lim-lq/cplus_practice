#ifndef __THREADPOO_H__
#define __THREADPOO_H__

#include <stdint.h>
#include <pthread.h>
#include <stdlib.h>

#include <iostream>
#include <string>
#include <tr1/memory>
#include <vector>

#include "queue.hpp"

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

template <typename Work, typename Task>
class ThreadPool : public Thread
{
public:
    ThreadPool(){}
    ~ThreadPool(){}

    int getWorkThread()
    {
        int thread_size = m_threads.size();
        for ( int i = 0; i < thread_size; ++i ) {
            if ( !m_threads[i]->isBusy() ){
                return i;
            }
        }
        return -1;
    }
    void create(const uint32_t thread_num = 10)
    {
        for ( uint32_t i = 0; i < thread_num; ++i ) {
            std::tr1::shared_ptr<const Work> thread(new Work(i));
            m_threads.push_back(thread);
        }
        for ( typename std::vector<std::tr1::shared_ptr<Work> >::iterator it = m_threads.begin();
              it != m_threads.end(); ++it ) {
            (*it)->start();
        }        
    }
    void push_task(const Task& task)
    {
        if ( m_threads.size() == 0 ) {
            std::cout << "There is no work thread, please create it first." << std::endl;
            exit(1);
        }
        m_taskQueue.put(task);
    }

    void run()
    {
        while ( true ) {
            Task task = m_taskQueue.get();
            int thread_id;
            while ( (thread_id = getWorkThread()) == -1 ) {
            }
            m_threads[thread_id]->assign_task(task);
        }
    }

private:
    Queue<Task> m_taskQueue;
    std::vector<std::tr1::shared_ptr<Work> > m_threads;
}; // end class ThreadPool

} // end namespace wind
#endif