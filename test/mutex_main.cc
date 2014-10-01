#include "mutex.h"
#include <iostream>

using namespace std;
using namespace coco;

int main(int argc, char* argv[])
{
    MyMutex mymutex;
    mymutex.run();
    return 0;
}