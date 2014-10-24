#ifndef __CONFIGURE_WRAPPER_H__
#define __CONFIGURE_WRAPPER_H__

#include <string>
#include <vector>
#include <tr1/memory>
#include "queue.h"
#include "configureServerInterface.h"

namespace  blond {

struct ClientInfo{
    ClientInfo(){}
    
};

class Connector : public IceUtil::Thread
{
public:
    Connector(std::vector<int>* clients, IceUtil::Mutex* mutex) : m_clientfdsPtr(clients), m_mutexPtr(mutex) {}
    virtual void run();
private:
    std::vector<int>* m_clientfdsPtr;
    IceUtil::Mutex* m_mutexPtr;
};

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
    void startConnectThread();

private:
    std::string m_key;
    Queue<int> _clientfds;
    std::vector<int> m_clientfds;
    IceUtil::Mutex m_mutex;
    std::tr1::shared_ptr<Connector> m_connector;
};
} // end namespace blond
#endif