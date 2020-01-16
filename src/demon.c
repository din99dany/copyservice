#include <string.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <queue.h>

///Mutex control shared stuff
pthread_mutex_t lockQueue;

void ReadCfg(char *filename, int *NrThreads, int *NrJobs);
void* Workingfunction( void* v );
int IsActiveJob( struct job* toCheck);
void AddvanceJob( struct job* workingJob );
void RepushJob( struct job* toAdd );

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

///Shared stuff
int Status[10000];
struct job JobHistory[10000];
struct queue Q; ///job queue

void Demon( )
{

    Q.first = 0;
    Q.last = -1;
    int id_cnt = 0; ///number of current jobs
    int NrThreads, NrJobs;

    ReadCfg("config.cfg", &NrThreads, &NrJobs); ///reading data from the config file
    pthread_mutex_init( &lockQueue, NULL );

    int sockfd, newsockfd, portno, pid;
    socklen_t clientConnection;
    struct sockaddr_un serv_addr = { AF_UNIX, "testserver\0" }, clientAdress;

    sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
    bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr));

    ///Creating the thread pool
    pthread_t threads[ NrThreads ];
    int threadsIds[ NrThreads ];
    for ( int i = 0; i < NrThreads; ++i )
    {
        threadsIds[ i ] = i;
        pthread_create( &threads[i], NULL, Workingfunction, (void*)&(threadsIds[ i ]) );
    }

    printf("threads init\n");

     listen(sockfd,1);
     clientConnection = sizeof(clientAdress);
     while (1)
     {
        printf("listen ...\n");
        ///Accepting a new client
        newsockfd = accept(sockfd,
            (struct sockaddr *) &clientAdress, &clientConnection);

        ///Receiving the command
        int command;
        read( newsockfd, &command, sizeof(command));

        printf("Received command %d : \n",command);

        ///if the maximum number of jobs is not reached
        if (id_cnt < NrJobs)
            switch (command)
            {
            case CREATE_JOB:
                {
                struct job citit;
                id_cnt++;
                write(newsockfd, &id_cnt, sizeof(id_cnt));
                read( newsockfd, &citit, sizeof(struct job));
                printf("%s %s\n",citit.src,citit.dest);
                pthread_mutex_lock(&lockQueue);
                    ///Adding the new job to the job queue
                    push( citit, &Q );
                    Status[citit.id] = citit.status;
                    JobHistory[ citit.id ] = citit;
                pthread_mutex_unlock(&lockQueue);
                break;
                }
            case LIST_ALL:
                {
                ///
                write(newsockfd, &id_cnt, sizeof(int));
                break;
                }
            case LIST_JOB:
                {
                int jobjob_id = 0;
                read( newsockfd, &jobjob_id, sizeof(int));
                write( newsockfd, &(JobHistory[jobjob_id]),sizeof(struct job));
                break;
                }
            default:
                {
                struct job citit_stat;
                read( newsockfd, &citit_stat, sizeof(struct job));
                pthread_mutex_lock(&lockQueue);
                    printf("%d %d\n",citit_stat.id,citit_stat.status);
                    Status[ citit_stat.id ] = citit_stat.status;
                    JobHistory[ citit_stat.id ].status = citit_stat.status;
                pthread_mutex_unlock(&lockQueue);
                break;
                }
            }
        close(newsockfd);
     }

    close(sockfd);
    return 0;
}

int main(int argc, char* argv[])
{
    FILE *fp= NULL;
    pid_t process_id = 0;
    pid_t sid = 0;
    // Create child process
    process_id = fork();
    // Indication of fork() failure
    if (process_id < 0)
    {
        printf("fork failed!\n");
        // Return failure in exit status
        exit(1);
    }
    // PARENT PROCESS. Need to kill it.
    if (process_id > 0)
    {
        printf("process_id of child process %d \n", process_id);
        // return success in exit status
        exit(0);
    }
    //unmask the file mode
    umask(0);
    //set new session
    sid = setsid();
    if(sid < 0)
    {
    // Return failure
    exit(1);
    }
    // Change the current working directory to root.
    chdir("./");
    // Close stdin. stdout and stderr
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
    // Open a log file in write mode.
    fp = fopen ("Log.txt", "w+");
    
    while (1)
    {
        //Dont block context switches, let the process sleep for some time
        sleep(1);
        fprintf(fp, "ok\n");
        fflush(fp);
        // Implement and call some function that does core work for this daemon.
        Demon();
    }
    fclose(fp);
    return (0);
}


void ReadCfg(char *filename, int *NrThreads, int *NrJobs)
{
    FILE *file = fopen(filename, "r");
    char s[30], *s1;

    fscanf(file, "%s", s);
    s1 = strchr(s, '=');
    sscanf(s1+1, "%d", &(*NrThreads));

    fscanf(file, "%s", s);
    s1 = strchr(s, '=');
    sscanf(s1+1, "%d", &(*NrJobs));
}

int SelectJob( struct job* toSelect)
{

    pthread_mutex_lock(&lockQueue);
    if ( !empty(&Q) )
    {
        (*toSelect) = front(&Q);
        pop(&Q);
        pthread_mutex_unlock(&lockQueue);
    }
    else
    {
        pthread_mutex_unlock(&lockQueue);
        return 0;
    }

    return 1;
}

void* Workingfunction( void* v)
{
    int threadId = *((int*)v);
    while ( 1 )
    {
        struct job workingJob;
        if ( SelectJob( &workingJob) )
        {
            int repush = 1;
            int maxInterations = 100;
            while (  IsActiveJob( &workingJob ) )
            {
                printf( "I was thread %d %s %s %f\n",
                    threadId,
                    workingJob.src,
                    workingJob.dest,
                    (float)(workingJob.buffer)/workingJob.fullsize*100
                );
                printf("------------------------\n");

                AddvanceJob(&workingJob);
                maxInterations--;
                if ( workingJob.buffer == workingJob.fullsize )
                {
                    repush = 0;
                    break;
                }
                if ( maxInterations <= 0 )
                {
                    break;
                }
            }

            if (repush)
            {
                RepushJob( &workingJob );
            }
        }
    }
}


int IsActiveJob( struct job* toCheck)
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

void RepushJob( struct job* toAdd )
{
    pthread_mutex_lock( &lockQueue );
        push(*toAdd, &Q);
    pthread_mutex_unlock( &lockQueue );
}

void AddvanceJob( struct job* workingJob )
{

    int fd_s = open( workingJob->src, O_RDONLY );
	int fd_d = open( workingJob->dest, O_WRONLY );

    char s[15];

    int bRead = pread( fd_s, s, 10, workingJob->buffer );
    pwrite( fd_d, s, bRead, workingJob->buffer );

    workingJob->buffer += bRead;
    JobHistory[workingJob->id].buffer += bRead;

    close(fd_s);
    close(fd_d);
}