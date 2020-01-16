#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include "spellcaster.h"

int main(int argc, char *argv[])
{
    
    struct  job job4 = CreateJob("./test9.txt","./test10.txt");
    int n = 15;
    int x = 10;
    PrintAllJobs();
    while (!n)
    {
        x = x * 10;
        PauseJob(&job4);
        n--;
        StartJob(&job4);
    }
    PrintJob(job4);

    return 0;
}