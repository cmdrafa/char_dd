#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<fcntl.h>
#include<string.h>
#include<unistd.h>

int main(int argc, char *argv[])
{
    int fd;
    char *path = "/dev/echo";

    fd = open(path, O_RDWR);
    if(fd < 0){
        perror("Opening device: ");
    }

    fd = close(fd);

    return 0;
}