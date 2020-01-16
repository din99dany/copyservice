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
        pid_t pdi2 = fork();
        if ( pdi2 == 0 )
            execv( "./2jobtest", NULL );
        else
        {
            
            pid_t pid3 = fork();

            if ( pid3 == 0 )
            {
                execv( "./3jobtest", NULL );
            } 
            else 
            {
                pid_t pid4 = fork();
                if ( pid4 == 0 )
                {
                    execv( "./4jobtest", NULL );
                } 
                else
                {
                    
                    pid_t pid5 = fork();

                    if ( pid5 == 0 )
                    {
                        execv( "./5jobtest", NULL );
                    } 
                    else
                    {
                        execv( "./6jobtest", NULL );
                    }
                    
                }
                
            }
        }
    }

    return 0;
}