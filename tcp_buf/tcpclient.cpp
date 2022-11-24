#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <errno.h>
#include <stdarg.h>
#include <syslog.h>
#include <arpa/inet.h>

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

# define MESSAGE_SIZE 10240000

void sendData(int sockFd)
{
    char *query;
    query = (char *)malloc(MESSAGE_SIZE + 1);
    for (int i = 0; i < MESSAGE_SIZE; i++) {
        query[i] = 'a';
    }
    query[MESSAGE_SIZE] = '\0';
    fprintf(stdout, "init buffer siz %ld \n", (long)MESSAGE_SIZE);
    const char *cp;
    cp = query;
    size_t remaining = strlen(query);
    while (remaining) {
        int n_written = send(sockFd, cp, remaining, 0);
        fprintf(stdout, "send into buffer %ld \n", n_written);
        if (n_written <= 0) {
            error(1, errno, "send failed");
            return;
        }
        remaining -= n_written;
        cp += n_written;
    }
    fprintf(stdout, "send into buffer success \n");
    return;
}

int main(int argc, char **argv)
{
    if (argc != 2) {
        error(1, 0, "usage: tcpclient <IPaddress>");
    }
    int sockfd;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in servaddr;
    bzero(&servaddr, sizeof (servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(12345);
    inet_pton(AF_INET, argv[1], &servaddr.sin_addr);
    int connect_rt = connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr));
    if (connect_rt < 0) {
        error(1, errno, "connect failed ");
    }
    sendData(sockfd);
    fprintf(stdout, "send into exit \n");
    exit(0);
}

