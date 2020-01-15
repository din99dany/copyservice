#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include "job.h"

int main(int argc, char *argv[])
{
    
    //struct  job nouCret2 = CreateJob("./test3.txt","./test4.txt");
    struct  job nouCret5 = CreateJob("./test5.txt","./test6.txt");

    PrintAllJobs();

    sleep(2);
    PauseJob(&nouCret5);
    PrintJob( nouCret5 );
    StartJob(&nouCret5);
    PrintJob( nouCret5 );
    CancelJob(&nouCret5);

    PrintAllJobs();

    return 0;
}