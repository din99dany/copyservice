#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include "job.h"

int main()
{

    pid_t pid = fork();
    if ( pid == 0 )
        execv( "./test", NULL );
    else
    {
        pid_t pid2 = fork();
        if ( pid2 == 0 )
            execv( "./test", NULL );
        else
        {
            pid_t pid3 = fork();
            if ( pid3 == 0 )
                execv( "./test", NULL );
            else
            {
                execv( "./test", NULL );
            }
        }
    }

    return 0;
}