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
    
    struct  job job5 = CreateJob("./test5.txt","./test6.txt");

    int i = 0;
    int j = i + 1;

    for (i = 1; i * i <= 1000; i = i + 2)
    {
        PauseJob(&job5);
        if ((i * j) % 4)
            StartJob(&job5);
    }
    
    PrintJob(job5);
    
    return 0;
}