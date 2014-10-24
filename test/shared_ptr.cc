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

template <typename T>
class ArrayDeleter{
public:
    void operator() (T* d) const
    {
        delete [] d;
        cout << "delete" << endl;
    }
};


int main(int argc, char* argv[])
{
    // B* b = new B();
    std::tr1::shared_ptr<B> ptrb(new B());
    std::tr1::shared_ptr<double> my_array(new double[256], ArrayDeleter<double>());
    my_array.get()[20] = 23.22;

    for ( int i = 0; i < 256; ++i ) {
        cout << my_array.get()[i] << endl;
    }
    return 0;
}