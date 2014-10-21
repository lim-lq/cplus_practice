#ifndef __CONFIGURE_WRAPPER_H__
#define __CONFIGURE_WRAPPER_H__

#include <string>
#include <vector>
#include "queue.h"
#include "configureServerInterface.h"

namespace  blond {
class ConfigureWrapper : public Blond::ConfigureServerInterface
{
public:
    ConfigureWrapper()
    {
        // m_mutex = IceUtil::Mutex();
    }
    ~ConfigureWrapper(){}

    ::std::string getkey(const Ice::Current& = Ice::Current());
    ::Ice::Int setkey(const std::string& key, const Ice::Current& = Ice::Current());
    void send_command(const std::string& cmd);
private:
    std::string m_key;
    Queue<int> _clientfds;
    std::vector<int> m_clientfds;
    IceUtil::Mutex m_mutex;
};
} // end namespace blond
#endif