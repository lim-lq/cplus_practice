#include <iostream>
#include <tr1/memory>

using namespace std;

class A
{
public:
    A()
    {
        cout << "Construct A!!!" << endl;
    }
    ~A()
    {
        cout << "Destruct A!!!" << endl;
    }
};

class B : public A
{
public:
    B()
    {
        cout << "Construct B!!!" << endl;
    }
    ~B()
    {
        cout << "Destruct B!!!" << endl;
    }
};

int main(int argc, char* argv[])
{
    // B* b = new B();
    std::tr1::shared_ptr<B> ptrb(new B());
    return 0;
}