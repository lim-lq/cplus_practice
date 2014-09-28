#include <sstream>
#include "server.h"

using namespace std;
using namespace simplechat;

int main(int argc, char* argv[])
{
    string host = "0.0.0.0";
    unsigned short port = 8000;
    int opt;
    while ( (opt = getopt(argc, argv, "h:p:")) != -1 ) {
        switch ( opt ) {
        case 'h':
            host = optarg;
            break;
        case 'p':
            istringstream iss(optarg);
            iss >> port;
            break;
        }
    }

    ChatServer server(host, port);
    server.wait();
    return 0;
}