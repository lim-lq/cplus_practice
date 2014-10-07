#include "configParser.hpp"
#include "utils.hpp"

using namespace std;

int main(int argc, char* argv[])
{
    ConfigParser config;
    string name = "women";
    config.load("test.conf");
    config.getAll();
    return 0;
}