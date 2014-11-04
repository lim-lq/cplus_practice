#include "dbmgr.h"
#include "log.hpp"
#include "configureParser.hpp"

#include <stdint.h>

#include <string>
#include <stdexcept>
#include <iostream>

using namespace std;
using namespace wind;

int main(int argc, char* argv[])
{
    wind::common::LoggerInitialize("dbmgr.log");
    ConfigureParser& config = SingletonConfigureParser::instance();
    config.read("dbmgr.config");

    string host;
    uint16_t port;

    if ( config.get("dbmgr", "host").empty() ) {
        throw std::invalid_argument("Configure file no dbmgr-host");
    }
    host = config.get("dbmgr", "host");

    if ( config.get("dbmgr", "port").empty() ) {
        throw std::invalid_argument("Configure file no dbmgr-port");
    }
    port = config.getInt16("dbmgr", "port");

    DBmgr server(host, port);
    server.run();

    return 0;
}