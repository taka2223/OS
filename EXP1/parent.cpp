#include <iostream>
#include <time.h>
#include <unistd.h>

int main()
{
    pid_t PID;
    PID = fork();
    if (PID == -1) {
        std::cout << "error!" << std::endl;
    } else if (PID == 0) {
        while (1) {
            time_t time1;
            time(&time1);
            std::cout << "The child is talking at " << (asctime(gmtime(&time1))) << std::endl;
            usleep(1000000);
        }
    } else {
        while (1) {
            time_t time2;
            time(&time2);
            std::cout << "The parent is talking at " << (asctime(gmtime(&time2))) << std::endl;
            usleep(1000000);
        }
    }
}