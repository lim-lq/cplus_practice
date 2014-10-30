#include <iostream>
#include <sstream>

#include "log.hpp"
#include "loginapp.h"

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

    wind::common::LoggerInitialize("loginapp.log");
    ThreadPool<WorkThread, Task> threadpool;
    threadpool.run();
    LoginApp app(host, port);
/*    app.run();*/

    return 0;
}