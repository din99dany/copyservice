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
    
    struct  job job3 = CreateJob("./test7.txt","./test8.txt");
    int a = 2;
    int b = 5;
    int i = 0;
    for (i = 10; i <= 9765; ++i)
    {
        if ( i % 2 == 0 && i % 5 == 0)
        {
            PauseJob(&job3);
        }
        StartJob(&job3);
    }

    return 0;
}