#include <iostream>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
using namespace std;

void* print_info(void* tid)
{
    int i = 10;
    pthread_t TID = *((pthread_t*)tid);
    while (i--) {
        time_t time1;
        time(&time1);
        cout << "TID: " << tid << " TIME: " << (asctime(gmtime(&time1))) << endl;
        usleep(1000000);
    }
    return NULL;
}
int main()
{
    pthread_t tid;
    pthread_create(&tid, NULL, print_info, (void*)&tid);
    pthread_t self = pthread_self();
    print_info((void*)&self);

    pthread_join(tid,NULL);
    pthread_exit(0);
}