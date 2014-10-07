#include <iostream>
#include <string>

using namespace std;

class Transport
{
public:
    Transport()
    {
        cout << "Transport constructor." << endl;
    }
    string ride()
    {
        cout << "You ride the ";
        return recv_msg();
    }
    void print_msg()
    {
        cout << "HSHSHSHSSHSHSH" << endl;
    }
    virtual string recv_msg() = 0;
};

class Vehicle : public Transport
{
public:
    Vehicle()
    {
        cout << "Vehicle constructor." << endl;
    }

    void riding(const string& msg)
    {
        print_msg();
        cout << "vehicle, very speed. -- " << msg << "." << endl;
    }
    string recv_msg()
    {
        string name = "I'm vehicle class.";
        return name;
    }
};

int main(int argc, char* argv[])
{
    Vehicle utils;
    // utils.ride();
    // utils.riding();
    char buf[1024] = "women";
    utils.riding(buf);
    string name = utils.ride();
    cout << "Recieve message :" << name << endl;
    return 0;
}