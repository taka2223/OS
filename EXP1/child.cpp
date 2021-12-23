#include "unistd.h"
#include <iostream>
#include <time.h>
using namespace std;

int main()
{
    while (1) {
        time_t time1;
        time(&time1);
        cout << "The child is talking at " << (asctime(gmtime(&time1)))<<endl;
        usleep(1000000);
    }
}