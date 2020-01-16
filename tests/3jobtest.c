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
        
    struct job jobjob  = CreateJob("./test11.txt","./test12.txt");

    int i = 0;

    for ( i= 10; i <= 100; ++i ) 
    {
        if ( i % 2 == 0 )
        {
            PauseJob(&jobjob);
        }
        StartJob(&jobjob);
    }

    return 0;
}