#include <iostream>
#include <sstream>
#include <stdexcept>

#include "log.hpp"
#include "loginapp.h"
#include "configureParser.hpp"

using namespace std;
using namespace wind;

int main(int argc, char* argv[])
{
    string host = "10.192.0.5";
    uint16_t port = 8000;

    int opt = 0;
    istringstream iss;
    while ( (opt = getopt(argc, argv, "i:p:h")) != -1 ) {
        switch ( opt ) {
        case 'i':
            host = optarg;
            break;
        case 'p':
            iss.str(optarg);
            iss >> port;
            break;
        case 'h':
            cout << "Usage: " << argv[0]
                 << " -i <host>"
                 << " -p <port>" << endl;
            return 0;
        default:
            break;
        }
    }

    ConfigureParser& config = SingletonConfigureParser::instance();
    config.read("loginapp.config");

    if ( config.get("loginapp", "host").empty() ) {
        throw std::invalid_argument("Configure file has no loginapp-host");
    }
    host = config.get("loginapp", "host");

    if ( config.get("loginapp", "port").empty() ) {
        throw std::invalid_argument("Configure file has no loginapp-port");
    }
    port = config.getInt16("loginapp", "port");

    wind::common::LoggerInitialize("loginapp.log");
    LoginApp app(host, port);
    app.run();

    return 0;
}