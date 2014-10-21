#include <Ice/Ice.h>
#include <iostream>

using namespace std;

int main(int argc, char* argv[])
{
    int status = 1;
    Ice::CommunicatorPtr ic;
    try {
        ic = Ice::initialize(argc, argv);
        status = 0;
    } catch (  ) {

    }

    if ( ic ) {
        try {
            ic->destory();
        } catch (  ) {

        }
    }

    return status;
}