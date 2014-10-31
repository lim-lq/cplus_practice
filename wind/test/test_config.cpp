#include "../common/configureParser.hpp"

#include <iostream>
#include <string>

#include <cstring>

using namespace std;
using namespace wind;

template <typename T>
const T& getS(const T& msg)
{
    return msg;
}

int main(int argc, char* argv[])
{
/*    ConfigureParser config;
    config.read("test.config");
    cout << config << endl;
    string value = config.get("spider", "size");
    cout << value << endl;*/

    string text = "blond";

    string msg = "blond";
    // strcpy(msg1, getS<char*>(msg2));
    string value = getS<string>(msg);
    cout << value << endl;
    return 0;
}