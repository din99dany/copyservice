/* A simple server in the internet domain using TCP
   The port number is passed as an argument 
   This version runs forever, forking off a separate 
   process for each connection
*/
 
#include <stdio.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include "job.h"
#include <queue>

void dostuff(int); /* function prototype */
void error(const char *msg)
{
    perror(msg);
    exit(1);
}

std::queue< struct job > Q;

int main( )
{
     int sockfd, newsockfd, portno, pid;
     socklen_t clilen;
     struct sockaddr_un serv_addr = { AF_UNIX, "testserver\0" }, cli_addr;

     sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
     bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr));

     listen(sockfd,5);
     clilen = sizeof(cli_addr);
     while (1) {
        printf("listen ...\n");
        newsockfd = accept(sockfd, 
            (struct sockaddr *) &cli_addr, &clilen);

        struct job citit;
        read( newsockfd, &citit, sizeof(job) );
        Q.push( citit );

         pid = fork();
         if (pid < 0)
             error("ERROR on fork");
         if (pid == 0)  {
             close(sockfd);
             dostuff(newsockfd);
             exit(0);
         }
         else close(newsockfd);
     } 
     close(sockfd);
     return 0;
}


void dostuff (int sock)
{

    std::queue< struct job > copy;
    copy = Q;

    while ( !copy.empty() )
    {   
        struct job topper = copy.front();
        printf( "%s %s\n", topper.src, topper.dest );
        copy.pop();
    }
    

}