#ifndef __HELLOI_H__
#define __HELLOI_H__

#include "Hello.h"

class HelloI : public Demo::Hello
{
public:
    HelloI(){}
    ~HelloI(){}
    void sayHello(int delay, const Ice::Current&);
    void shutdown(const Ice::Current&);
};

#endif