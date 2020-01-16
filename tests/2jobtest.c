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
    struct job nouCret34 = CreateJob("./test3.txt","./test4.txt");

    PrintAllJobs();

    return 0;
}