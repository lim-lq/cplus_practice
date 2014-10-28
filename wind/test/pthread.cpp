#include <pthread.h>
#include <iostream>

using namespace std;

pthread_mutex_t lock1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lock2 = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond1 = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond2 = PTHREAD_COND_INITIALIZER;

void *work(void* arg)
{
    while ( true ) {
        pthread_mutex_lock(&lock1);
        pthread_cond_wait(&cond1, &lock1);
        cout << "I get the cond1 variable, I'm the thread process." << endl;
        pthread_mutex_unlock(&lock1);

/*        pthread_mutex_lock(&lock2);
        pthread_cond_signal(&cond2);
        cout << "Change condition variable [cond2], I'm the thread process." << endl;
        pthread_mutex_unlock(&lock2);*/
    }
}

int main(int argc, char* argv[])
{
    pthread_t tid;

    pthread_create(&tid, NULL, work, NULL);
    cout << "Thread begin running." << endl;
    int times = 1;

    while ( times ) {
        pthread_mutex_lock(&lock1);
        pthread_cond_signal(&cond1);
        cout << "Change condition variable [cond1], I'm the main thread." << endl;
        pthread_mutex_unlock(&lock1);

/*        pthread_mutex_lock(&lock2);
        pthread_cond_wait(&cond2, &lock2);
        cout << "I get the condition variable [cond2], I'm the main thread." << endl;
        pthread_mutex_unlock(&lock2);*/
        ++times;
    }
    pthread_mutex_destroy(&lock1);
    pthread_mutex_destroy(&lock2);
    pthread_cond_destroy(&cond1);
    pthread_cond_destroy(&cond2);
    return 0;
}