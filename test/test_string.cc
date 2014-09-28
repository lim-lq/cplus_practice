#include <iostream>
#include <string>

using namespace std;

inline void strip(std::string& str)
{
    std::string::iterator it = str.begin();

    if ( *(it + str.size() - 1) == '\n' ) {
        str.erase(it + str.size() - 1);
    }
}

int main(int argc, char* argv[])
{
    string name = "liqing\n";
    cout << name;
    strip(name);
    cout << name;
    return 0;
}