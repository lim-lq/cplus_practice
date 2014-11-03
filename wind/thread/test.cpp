#include "threadpool.h"

#include <iostream>

using namespace std;
using namespace wind;

int main(int argc, char* argv[])
{
    Thread thread = Thread();
    int ret = thread.create();

    if ( ret != 0 ) {
        std::cout << "Create thread failure, error code is [" << ret << "]." << endl;
        return -1;
    }

    while ( true ) {
        sleep(3);
        cout << "I'm the main process." << endl;
    }

    return 0;
}