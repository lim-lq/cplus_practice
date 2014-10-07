#include <iostream>
// #include <pthread.h>
#include "mutex.h"

using namespace std;
using namespace coco;

pthread_mutex_t _lock;

void* thread_worker(void *arg)
{
    while ( true ) {
        pthread_mutex_lock(&_lock);
        cout << "I'm the thread process" << endl;
        pthread_mutex_unlock(&_lock);
        sleep(2);
    }
}

int main(int argc, char* argv[])
{
/*    pthread_mutex_lock(&_lock);
    cout << "I get the lock" << endl;
    pthread_mutex_unlock(&_lock);
    cout << "I free the lock" << endl;*/
/*    pthread_t tid;
    pthread_create(&tid, NULL, &thread_worker, NULL);

    while ( true ) {
        pthread_mutex_lock(&_lock);
        cout << "I'm the main process" << endl;
        pthread_mutex_unlock(&_lock);
        sleep(2);
    }*/
    MyMutex mymutex(3);
    mymutex.run();
    return 0;
}