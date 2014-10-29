#include <iostream>
#include "../threadPool.h"
#include <vector>

#define THREAD_NUM  5

using namespace std;
using namespace wind;

int main(int argc, char* argv[])
{
    ThreadPool threadpool;

    threadpool.create(10);

    uint32_t times = 0;

    while ( true ) {
        cout << "I'm the main process." << endl;
        ++times;
        threadpool.push_task("aaa");
        threadpool.push_task("bbb");
        threadpool.push_task("ccc");
        threadpool.push_task("ddd");
        threadpool.push_task("eee");
        threadpool.push_task("fff");
        threadpool.push_task("ggg");
        threadpool.push_task("hhh");
        threadpool.push_task("iii");
        threadpool.push_task("jjj");
        threadpool.push_task("kkk");
        threadpool.push_task("lll");
        threadpool.push_task("mmm");
    }
    return 0;
}