#include <stdio.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <queue>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "job.h"
#include "pthread.h"

pthread_mutex_t lockQueue;

void* workingfunction( void* v );
void error(const char *msg)
{
    perror(msg);
    exit(1);
}

std::queue< struct job > Q;
int Status[10000];
struct job JobHistory[10000];


int main( )
{       
    int id_cnt = 0;
    pthread_mutex_init( &lockQueue, NULL );

     int sockfd, newsockfd, portno, pid;
     socklen_t clilen;
     struct sockaddr_un serv_addr = { AF_UNIX, "testserver\0" }, cli_addr;

     sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
     bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr));


    pthread_t thr[ 6 ];
    int ids[ 6 ];
    for ( int i = 0; i < 5; ++i )
    {
        ids[ i ] = i;
        pthread_create( &thr[i], NULL, workingfunction, (void*)&(ids[ i ]) );
    }

    printf("threads init\n");

     listen(sockfd,1);
     clilen = sizeof(cli_addr);
     while (1) {
        printf("listen ...\n");
        newsockfd = accept(sockfd, 
            (struct sockaddr *) &cli_addr, &clilen);


        int command;
        read( newsockfd, &command, sizeof(command));

        printf("Received command %d : \n",command);

        switch (command)
        {
        case CREATE_JOB:
        {    struct job citit;
            id_cnt++;
            write(newsockfd, &id_cnt, sizeof(id_cnt));
            read( newsockfd, &citit, sizeof(struct job));
            printf("%s %s\n",citit.src,citit.dest);
            pthread_mutex_lock(&lockQueue);
                Q.push( citit );
                Status[citit.id] = citit.status;
                JobHistory[ citit.id ] = citit;
            pthread_mutex_unlock(&lockQueue);
            break;
        }
        case LIST_ALL:
            write(newsockfd,&id_cnt,sizeof(int));
            break;
        case LIST_JOB:
        {
            int jobjob_id = 0;
            read( newsockfd, &jobjob_id, sizeof(int));
            write( newsockfd, &(JobHistory[jobjob_id]),sizeof(struct job));
            break;
        }
        default:
            struct job citit_stat;
            read( newsockfd, &citit_stat, sizeof(struct job));
            pthread_mutex_lock(&lockQueue);
                printf("%d %d\n",citit_stat.id,citit_stat.status);
                Status[ citit_stat.id ] = citit_stat.status;
                JobHistory[ citit_stat.id ].status = citit_stat.status;
            pthread_mutex_unlock(&lockQueue);
            break;
        }
        close(newsockfd);
     } 

     close(sockfd);
     return 0;
}

bool SelectJob( struct job* toSelect)
{

    pthread_mutex_lock(&lockQueue);
    if ( !Q.empty() )
    {
        (*toSelect) = Q.front();
        Q.pop();
        pthread_mutex_unlock(&lockQueue);
    }
    else
    {
        pthread_mutex_unlock(&lockQueue);
        return false;
    }
    
    return true;
}

bool IsActiveJob( struct job* toCheck);
void AddvanceJob( struct job* toCopy );
void RepushJob( struct job* toCopy );

void* workingfunction( void* v)
{   
    int cnt = *((int*)v);
    while ( true )
    {
        struct job copyJob;
        if ( SelectJob( &copyJob) )
        {   
            bool repush = true;
            while (  IsActiveJob( &copyJob ) )
            {   
                printf( "I was thread %d %s %s %f\n",cnt, copyJob.src,copyJob.dest,1.0*copyJob.buffer/copyJob.fullsize*100);
                AddvanceJob(&copyJob);
                printf("------------------------\n");
                if ( copyJob.buffer == copyJob.fullsize )
                {
                    repush = false;
                    break;
                }
            }

            if ( repush )
            {
                RepushJob( &copyJob );
            }

        }
    }
}


bool IsActiveJob( struct job* toCheck)
{   
    pthread_mutex_lock(&lockQueue);
        toCheck->status = Status[ toCheck->id ];
    pthread_mutex_unlock(&lockQueue);

    if ( toCheck->status == -1 )
    {
        toCheck->buffer = toCheck->fullsize;
        JobHistory[ toCheck->id ].buffer = toCheck->fullsize;    
    }

    return toCheck->status != 0;
}

void RepushJob( struct job* toCopy )
{
    pthread_mutex_lock( &lockQueue );
        Q.push(*toCopy);
    pthread_mutex_unlock( &lockQueue );
}

void AddvanceJob( struct job* toCopy )
{

    int fd_s = open( toCopy->src, O_RDONLY );
	int fd_d = open( toCopy->dest, O_WRONLY );

    char s[100];

    int bRead = pread( fd_s, s, 10, toCopy->buffer );
    pwrite( fd_d, s, bRead, toCopy->buffer );
    toCopy->buffer += bRead;
    JobHistory[toCopy->id].buffer += bRead;

    close(fd_s);
    close(fd_d);
}