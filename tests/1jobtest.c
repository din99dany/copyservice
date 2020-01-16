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
    
    struct  job nouCret5 = CreateJob("./test1.txt","./test2.txt");

    PrintAllJobs();
    PauseJob(&nouCret5);
    StartJob(&nouCret5);
    PrintJob( nouCret5 );

    return 0;
}