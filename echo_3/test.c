#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    int fd, rc_w, rc_r;
    int bufsize = strlen(argv[1]);
    char *buf = malloc(bufsize);
    char *path = "/dev/echo";

    printf("Writting %d bytes to char device \n", bufsize);

    buf = argv[1];

    fd = open(path, O_RDWR);
    if (fd < 0)
    {
        perror("Opening device: ");
    }

    rc_w = write(fd, buf, bufsize);
    rc_r = read(fd, buf, bufsize);
    printf("RC_R: %d\n", rc_r);


    fd = close(fd);

    return 0;
}