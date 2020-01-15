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
    
    //struct  job nouCret = CreateJob("./test1.txt","./test2.txt");
    struct  job nouCret2 = CreateJob("./test3.txt","./test4.txt");
    //struct  job nouCret5 = CreateJob("./test5.txt","./test6.txt");
    
    // PuaseJob( nouCret );
    // PuaseJob( nouCret2 );
    // PuaseJob( nouCret5 );

    // PuaseJob( nouCret );
    // StartJob( nouCret2 );
    // StartJob( nouCret5 );
    sleep(2);
    CancelJob(&nouCret2);

    return 0;
}