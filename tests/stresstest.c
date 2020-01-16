#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include "spellcaster.h"

int main()
{

    pid_t pid = fork();
    if ( pid == 0 )
        execv( "./1jobtest", NULL );
    else
    {
        execv( "./2jobtest", NULL );
    }

    return 0;
}