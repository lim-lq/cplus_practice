#ifndef __QUEUE_H__
#define __QUEUE_H__

#include <IceUtil/Monitor.h>
#include <IceUtil/Mutex.h>
#include <list>

namespace blond{
template<class T>
class Queue{
public:
    T get() {
        IceUtil::Monitor<IceUtil::Mutex>::Lock lock(m_monitor);
        while ( m_queue.empty() ) {
            m_monitor.wait();
        }
        T item = m_queue.front();
        m_queue.pop_front();
        return item;
    }

    void put(const T& item) {
        IceUtil::Monitor<IceUtil::Mutex>::Lock lock(m_monitor);
        m_queue.push_back(item);
        m_monitor.notify();
    }

private:
    std::list<T> m_queue;
    IceUtil::Monitor<IceUtil::Mutex> m_monitor;
};
} // end namespace blond

#endif