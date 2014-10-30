/*
 * 2014-10-29
 * author: blond.li
 */

#include <iostream>
#include <sstream>
#include <stdexcept>

#include <stdlib.h>

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

/*template <typename Work, typename Task>
void ThreadPool<Work, Task>::run()
{
    std::cout << "I'm the run function." << std::endl;
}*/

/*template <typename Work, typename Task>
int ThreadPool<Work, Task>::getWorkThread()
{
    int thread_size = m_threads.size();
    for ( int i = 0; i < thread_size; ++i ) {
        if ( !m_threads[i]->isBusy() ){
            return i;
        }
    }
    return -1;
}

template <typename Work, typename Task>
void ThreadPool<Work, Task>::create(const uint32_t thread_num)
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

template <typename Work, typename Task>
void ThreadPool<Work, Task>::push_task(const Task& task)
{
    if ( m_threads.size() == 0 ) {
        std::cout << "There is no work thread, please create it first." << std::endl;
        exit(1);
    }
    m_taskQueue.put(task);
}*/

} // end namespace wind