#ifndef __TASK_HPP__
#define __TASK_HPP__

namespace wind
{
class Task
{
public:
    Task(){}
    ~Task(){}
    virtual void run(){};
}; // end class Task
} // end namespace wind

#endif