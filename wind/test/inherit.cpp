#include <iostream>

using namespace std;

class Base
{
public:
    Base()
    {
        cout << "Base constructor." << endl;
    }
    virtual ~Base()
    {
        cout << "Base destructor." << endl;
    }
    virtual void run()
    {
        cout << "I'm running - Base class." << endl;
    };
};

class Derive : public Base
{
public:
    Derive()
    {
        cout << "Derive constructor." << endl;
    }
    ~Derive()
    {
        cout << "Derive destructor." << endl;
    }
    void run()
    {
        cout << "I'm running." << endl;
    }
};

class Work
{
public:
    Work(){}
    ~Work(){}
    void run()
    {
        m_task->run();
        delete m_task;
    }
    void setTask(Base* task)
    {
        m_task = task;
    }
private:
    Base* m_task;
};

int main(int argc, char* argv[])
{
    Work work;

    Derive* task = new Derive();

    work.setTask(task);

    work.run();

    while ( true ) {
        sleep(3);
    }
}