#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <stdarg.h>        /* ANSI C header file */
#include <syslog.h>        /* for syslog() */
#include <arpa/inet.h>
#include    <string.h>
#include    <unistd.h>



void error(int status, int err, char *fmt, ...) {
    va_list ap;

    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    va_end(ap);
    if (err)
        fprintf(stderr, ": %s (%d)\n", strerror(err), err);
    if (status)
        exit(status);
}

size_t readn(int socketFd, void *buf, size_t size)
{
    size_t nleft = size;
    char *ptr = (char *)buf;

    while (nleft > 0) {
        int result = read(socketFd, ptr, nleft);
        if (result  < 0) {
            if (errno = EINTR) {
                continue;
            } else {
                return -1;
            }
        } else if (result == 0) {
            break;
        }
        ptr += result;
        nleft -= result;
    }
    return size - nleft;
}

void read_data(int condFd)
{
    char buf[1024];
    int n;

    int times = 0;
    for (;;) {
        fprintf(stdout, "block in read\n");
        if ((n = readn(condFd, buf, 1024)) == 0) {
            fprintf(stdout, "read exit\n");
            return;
        }
        times++;
        fprintf(stdout, "1K read for %d \n", times);
        usleep(1000000);
    }
}

int main(int argc, char **argv) {
    int listenFd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in cliAddr, srvAddr;
    bzero(&srvAddr, sizeof (srvAddr));
    srvAddr.sin_family = AF_INET;
    srvAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    srvAddr.sin_port = htons(12345);

    bind(listenFd, (struct sockaddr *)&srvAddr, sizeof (srvAddr));
    listen(listenFd, 1024);

    for(; ;) {
        printf("accepted\n");
        socklen_t cliLen = sizeof (cliAddr);
        int connFd = accept(listenFd, (struct sockaddr *)&cliAddr, &cliLen);
        read_data(connFd);
        close(connFd);
    }
}