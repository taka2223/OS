#include <iostream>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
using namespace std;

int shared_var = 0;

void parent()
{
    while (1) {
        shared_var++;
        cout << "current value is " << shared_var << endl;
        usleep(1000000);
    }
}

void* child(void* args)
{
    while (1) {
        shared_var--;
        cout << "current value is " << shared_var << endl;
        usleep(1000000);
    }
    return NULL;
}

int main()
{
    pthread_t tid;
    pthread_create(&tid, NULL, child, NULL);
    parent();

    pthread_join(tid, NULL);
    pthread_exit(0);
}