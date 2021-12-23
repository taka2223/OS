#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>
#include <unistd.h>
using namespace std;

sem_t wmutex;
sem_t mutex;

int reader = 0;

void* read(void* i)
{
    pthread_t id = *((pthread_t*)i);
    while (1) {

        sem_wait(&mutex);
        reader++;
        if (reader == 1)
            sem_wait(&wmutex);
        sem_post(&mutex);

        cout << id << " is reading\n";
        usleep(1000000);
        cout << id << " have finished reading\n";

        sem_wait(&mutex);
        reader--;
        if (reader == 0)
            sem_post(&wmutex);
        sem_post(&mutex);
        return NULL;
    }
}
void* write(void* i)
{
    pthread_t id = *((pthread_t*)i);
    while (1) {
        sem_wait(&wmutex);
        cout << id << " is writing\n";
        usleep(6000000);
        cout << id << " have finished writing\n";
        sem_post(&wmutex);
        return NULL;
    }
    return NULL;
}

int main()
{
    sem_init(&wmutex, 0, 1);
    sem_init(&mutex, 0, 1);

    int i = 0;
    pthread_t R[20];
    pthread_t W[20];
    for (i = 0; i < 20; i++) {
        int a = rand() % 2;
        if (a == 0) {
            pthread_create(&R[i], NULL, read, (void*)&R[i]);
            usleep(500000 + rand() % 150000);
        } else {
            pthread_create(&W[i], NULL, write, (void*)&W[i]);
            usleep(500000 + rand() % 150000);
        }
    }

    pthread_exit(0);
    return 0;
}