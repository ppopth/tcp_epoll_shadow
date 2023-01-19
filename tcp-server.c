#include <stdio.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 8080

#define handle_error(msg) \
    do { perror(msg); exit(EXIT_FAILURE); } while (0)

int main()
{
    int sockfd, connfd, len;
    struct sockaddr_in servaddr, cli;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        handle_error("socket creation");
    }

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);

    if (bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) != 0) {
        handle_error("socket bind");
    }

    if (listen(sockfd, 5) != 0) {
        handle_error("listen");
    }

    connfd = accept(sockfd, (struct sockaddr*)&cli, &len);
    if (connfd < 0) {
        handle_error("server accept");
    }

    char *hello = "hello\n";
    for (;;) {
        sleep(2);
        write(connfd, hello, 6);
    }

    close(sockfd);
}
