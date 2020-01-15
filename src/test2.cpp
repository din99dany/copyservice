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
    for ( int i = 20; i >= 0; --i )
    {
    
        struct  job nouCret = CreateJob("./pacanea","./tese2");
        printf( "%s %s\n", nouCret.src, nouCret.dest );
    }
    
    for ( int i = 1; i < 999999999; ++i );

    return 0;
}