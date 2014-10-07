#include "socket_wrapper.h"
#include <sstream>
#include <stdlib.h>

using namespace std;
using namespace coco;

int main(int argc, char* argv[])
{
    string host = "192.168.200.128";
    unsigned short port = 8000;
/*    istringstream iss;
    int opt = 0;

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
            cout << "Usage: "
                 << argv[0]
                 << " -i <host>"
                 << " -p <port>."
                 << endl;
            return 0;
        default:
            break;
        }
    }*/

    SocketWrapper socketWrapper(host, port);
/*    int ret = socketWrapper.create_socket();
    if ( ret == -1 ) {
        exit(1);
    }*/
    socketWrapper.wait();

    return 0;
}