#include "template_test.h"

#include <iostream>

template<typename T, typename T2>
void Value<T, T2>::set(const T& t)
{
    m_item = t;
    std::tr1::shared_ptr<T> item(new T(1));
    m_items.push_back(item);
}

template<typename T, typename T2>
T Value<T, T2>::get()
{
    for ( typename std::vector<std::tr1::shared_ptr<T> >::iterator it = m_items.begin();
          it != m_items.end(); ++it ) {
        std::cout << *(*it) << std::endl;
    }
    return m_item;
}