#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ioctl.h>

struct job
{

    char src[126];
    char dest[126];
    int fdsocket;
    int id;
    int buffer;

};

job CreateJob( const char* src, const char* dest)
{
    int sock;
    int conn;

    struct sockaddr saddr = {AF_UNIX, "testserver\0"};
    socklen_t saddrlen = sizeof(struct sockaddr);
 
    sock = socket(AF_UNIX, SOCK_STREAM, 0);
    conn = connect(sock, &saddr, saddrlen);

    struct job toRet;

    strcpy( toRet.src, src );
    strcpy( toRet.dest, dest );
    toRet.buffer = 0;
    toRet.id = 0;
    toRet.fdsocket = sock;

    write( sock, &toRet, sizeof(toRet) );

    return toRet;

}