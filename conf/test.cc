#include "configparser.hpp"

using namespace std;

int main(int argc, char* argv[])
{
    ConfigParser parser;
    int ret = parser.read("test.conf");
    if ( ret ) {
        cout << "Read config file success" << endl;
        parser.getAll();
    } else {
        cout << "Read config file failure" << endl;
    }

    std::cout << parser.get("mysql", "db") << std::endl;

    return 0;
}