#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/stat.h>

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
        printf("cancel job...");
        break;
    default :
        break;
    }   
    printf("status %d \n",tomod->status);

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