#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <cstdio>

enum OPERATIONS{ CREATE_JOB, PAUSE_JOB, CANCEL_JOB, START_JOB, LIST_ALL, LIST_JOB };

struct job
{

    char src[126];
    char dest[126];
    int id;
    int buffer;
    int fullsize;
    int status;

};

int CreateSocket()
{

    int sock;
    int conn;

    struct sockaddr saddr = {AF_UNIX, "testserver\0"};
    socklen_t saddrlen = sizeof(struct sockaddr);
 
    sock = socket(AF_UNIX, SOCK_STREAM, 0);
    connect(sock, &saddr, saddrlen);

    return sock;
}

job CreateJob( const char* src, const char* dest)
{
    struct job toRet;    
    int sock = CreateSocket();
    int operation = CREATE_JOB;

    write( sock, &operation, sizeof(operation) );

    read( sock, &(toRet.id), sizeof(int));
    toRet.buffer = 0;
    toRet.status = 1;

    struct stat st;
    stat(src, &st);
    toRet.fullsize = st.st_size;

    strcpy( toRet.src, src );
    strcpy( toRet.dest, dest );


    write( sock, &toRet, sizeof(toRet) );

    close(sock);

    return toRet;

}

void ChangeStatus( struct job* tomod, int command )
{

    int sock = CreateSocket();
    write( sock, &command, sizeof(command) );
    
    switch (command)
    {
    case PAUSE_JOB:
        tomod->status = 0;
        break;
    case START_JOB:
        tomod->status = 1;
        break;
    case CANCEL_JOB :
        tomod->status = -1;
        break;
    default :
        break;
    }   

    write(sock, tomod, sizeof(struct job) );
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

    struct job toUpdate;
    
    int sock = CreateSocket();
    int comm = LIST_JOB;
    write( sock, &comm, sizeof(int) );

    write( sock,&(id), sizeof(int) );
    read( sock, &toUpdate, sizeof(struct job ) );

    printf( "Job %d:\n",  toUpdate.id );
    printf( "|--> src : %s \n", toUpdate.src );
    printf( "|--> dest : %s \n", toUpdate.dest );
    printf( "|--> progg : %f \n", toUpdate.buffer * 1.0 / toUpdate.fullsize * 100 );
    
    char* statuSide[3] = { "|-->status : canceled\n", "|-->status : paused\n", "|-->status : active\n" };
    printf( "%s", statuSide[ toUpdate.status + 1 ] );
}

void PrintJob( struct job toPrint)
{
    PrinntJobWithId( toPrint.id );
}

void PrintAllJobs()
{
    int sock = CreateSocket();
    int comm = LIST_ALL;

    write( sock, &comm, sizeof(int) );
    int n;
    read( sock, &n, sizeof(int) );

    close(sock);
    for ( int i = 1; i <= n; ++i )
    {
        PrinntJobWithId( i );
    }

}