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
    
    struct  job nouCret = CreateJob("./pacanea","./tese2");
    printf( "%s %s\n", nouCret.src, nouCret.dest );


    return 0;
}