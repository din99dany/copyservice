#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include "spellcaster.h"

int CreateSocket()
{

    int sock;
    struct sockaddr saddr = {AF_UNIX, "testserver\0"};
    socklen_t saddrlen = sizeof(struct sockaddr);
 
    sock = socket(AF_UNIX, SOCK_STREAM, 0);
    connect(sock, &saddr, saddrlen);

    return sock;
}

struct job CreateJob( const char* src, const char* dest)
{
    struct job newJob;    
    int sock = CreateSocket();
    int command = CREATE_JOB;

    write(sock, &command, sizeof(int));
    read(sock, &(newJob.id), sizeof(int));

    newJob.buffer = 0;
    newJob.status = 1;
    strcpy(newJob.src, src);
    strcpy(newJob.dest, dest);

    struct stat st;
    stat(src, &st);
    newJob.fullsize = st.st_size;

    write(sock, &newJob, sizeof(newJob));
    close(sock);

    return newJob;
}

void ChangeStatus( struct job* toCHange, int command )
{

    int sock = CreateSocket();
    write( sock, &command, sizeof(command) );
    
    switch (command)
    {
    case PAUSE_JOB:
        toCHange->status = 0;
        break;
    case START_JOB:
        toCHange->status = 1;
        break;
    case CANCEL_JOB :
        toCHange->status = -1;
        break;
    default :
        break;
    }   

    write(sock, toCHange, sizeof(struct job) );
    close(sock);
}

void CancelJob( struct job* tomod)
{
    ChangeStatus( tomod, (int)CANCEL_JOB);
}


void PauseJob( struct job* tomod)
{
    ChangeStatus( tomod, (int)PAUSE_JOB);
}

void StartJob( struct job* tomod)
{
    ChangeStatus( tomod, (int)START_JOB);
}

void PrinntJobWithId( int id )
{

    struct job toPrint;
    
    int sock = CreateSocket();
    int command = LIST_JOB;
    write( sock, &command, sizeof(int) );

    write( sock,&(id), sizeof(int) );
    read( sock, &toPrint, sizeof(struct job ) );

    printf( "Job %d:\n",  toPrint.id );
    printf( "|--> src : %s \n", toPrint.src );
    printf( "|--> dest : %s \n", toPrint.dest );
    printf( "|--> progg : %f \n", toPrint.buffer * 1.0 / toPrint.fullsize * 100 );
    
    char* statusBridge[3] = { "|-->status : canceled\n", "|-->status : paused\n", "|-->status : active\n" };
    printf( "%s", statusBridge[ toPrint.status + 1 ] );
}

void PrintJob( struct job toPrint)
{
    PrinntJobWithId( toPrint.id );
}

void PrintAllJobs()
{
    int sock = CreateSocket();
    int command = LIST_ALL;

    write( sock, &command, sizeof(int) );
    int numberOfJobs;
    read( sock, &numberOfJobs, sizeof(int) );

    close(sock);
    for ( int i = 1; i <= numberOfJobs; ++i )
    {
        PrinntJobWithId( i );
    }

}