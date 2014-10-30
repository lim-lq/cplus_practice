#ifndef __TEMPLATE_H__
#define __TEMPLATE_H__

#include <vector>
#include <tr1/memory>

template <typename T, typename T2>
class Value
{
public:
    void set(const T& t);
    T get();
private:
    T m_item;
    T2 m_item2;
    std::vector<std::tr1::shared_ptr<T> > m_items;
};

class MyValue
{
public:
    MyValue()
    {
        m_myItem.set(2);
        m_myItem.get();
    }
    ~MyValue(){}
private:
    Value<int, long> m_myItem;
};
#endif