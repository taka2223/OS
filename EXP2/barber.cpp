#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>
#include <unistd.h>
using namespace std;
sem_t customer;
sem_t barber;
sem_t mutex;


int seat = 6;
void* Barber(void* t)
{
    while (true) {
        cout << "Barber is sleeping" << endl;
        sem_wait(&customer);
        sem_wait(&mutex);
        seat++;
        sem_post(&barber);
        sem_post(&mutex);
        cout << "Barber is working" << endl;
    }
    return NULL;
}

void* Customer(void* t)
{
    unsigned long n  =*((unsigned long*)t);
    while (true) {
        sem_wait(&mutex);
        if (seat > 0) {
            seat--;
            sem_post(&customer);
            cout<<seat<<" seats"<<endl;
            sem_post(&mutex);
            sem_wait(&barber);
            cout<<"customer "<<n<<" hair cut"<<endl;
        }else{
            sem_post(&mutex);
            cout<<"Full"<<endl;
        }
    }
    
    return NULL;
}

int main(){
    sem_init(&customer,0,0);
    sem_init(&barber,0,0);
    sem_init(&mutex,0,1);
    pthread_t tid;
    pthread_create(&tid, NULL, Barber, NULL);
    int i=0;
    pthread_t C[20];
    for (i = 0; i < 20; i++)
    {
        pthread_create(&C[i],NULL,Customer,&i);
        usleep(1000000+rand()%150000);
    }
    for (size_t i = 0; i < 20; i++)
    {
        pthread_join(C[i],NULL);
    }
    
    pthread_exit(0);
    return 0;
    
}