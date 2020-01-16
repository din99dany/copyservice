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
    char src[226];
    char dest[226];
    int id;
    int buffer;
    int fullsize;
    int status;
};
