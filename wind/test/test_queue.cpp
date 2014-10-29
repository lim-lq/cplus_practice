#include <iostream>
#include <string>

#include <pthread.h>

#include "../common/queue.hpp"

using namespace std;
using namespace wind;

Queue<string> message_queue;

void* work(void * args)
{
    int* p_id = (int *)args;
    string msg;
    while ( true ) {
        msg = message_queue.get();
        cout << "Work-" << *p_id << " get message [" << msg << "]." << endl;
    }
}

int main(int argc, char* argv[])
{
    pthread_t tid1;
    pthread_t tid2;

    int id1 = 1;
    int id2 = 2;

    pthread_create(&tid1, NULL, work, &id1);
    pthread_create(&tid2, NULL, work, &id2);

    while ( true ) {
        string line;
        cout << "Please input message: ";
        cout.flush();
        std::getline(cin, line);
        message_queue.put(line);
    }
    return 0;
}