/*
 * 2014-10-30
 * author: blond.li
 * description: A simple queue complement
 * version: 1.0
 */

#ifndef __QUEUE_HPP__
#define __QUEUE_HPP__

#include <pthread.h>
#include <stdint.h>

#include <list>

namespace wind
{
template <typename T>
class Queue
{
public:
    Queue(const uint32_t& size = 4096) : m_IsEmpty(true), m_IsFull(false), m_qsize(size)
    {
        pthread_mutex_init(&m_lock, NULL);
        pthread_cond_init(&m_cond, NULL);
    }
    ~Queue()
    {
        pthread_mutex_destroy(&m_lock);
        pthread_cond_destroy(&m_cond);
    }
    void put(const T& item)
    {
        m_IsEmpty = m_queue.empty();
        m_queue.push_back(item);
        if ( m_IsEmpty && !m_queue.empty() ) {
            pthread_mutex_lock(&m_lock);
            pthread_cond_signal(&m_cond);
            pthread_mutex_unlock(&m_lock);
        }
    }
    T get()
    {
        while ( m_queue.empty() ) {
            pthread_mutex_lock(&m_lock);
            pthread_cond_wait(&m_cond, &m_lock);
            pthread_mutex_unlock(&m_lock);
        }
        T item = m_queue.front();
        m_queue.pop_front();
        return item;
    }

    bool empty() {
        return m_queue.empty();
    }

    bool full() {
        return m_IsFull;
    }
private:
    bool m_IsEmpty;
    bool m_IsFull;
    uint32_t m_qsize;
    std::list<T> m_queue;
    pthread_mutex_t m_lock;
    pthread_cond_t m_cond;
}; // end class Queue
} // end namespace wind

#endif