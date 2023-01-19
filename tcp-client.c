#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>
#define PORT 8080

#define handle_error(msg) \
    do { perror(msg); exit(EXIT_FAILURE); } while (0)

int main()
{
    int epollfd;
#define MAX_EVENTS 10
    struct epoll_event ev, events[MAX_EVENTS];
    int nfds;

    int sockfd;
    struct sockaddr_in servaddr;

    sockfd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC, 0);
    if (sockfd == -1) {
        handle_error("socket creation");
    }

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("1.1.1.1");
    servaddr.sin_port = htons(PORT);

    connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
    if (errno != EINPROGRESS) {
        handle_error("connection with the server");
    }

    epollfd = epoll_create1(EPOLL_CLOEXEC);
    ev.events = EPOLLIN | EPOLLRDHUP | EPOLLET;
    ev.data.fd = sockfd;
    if (epoll_ctl(epollfd, EPOLL_CTL_ADD, sockfd, &ev) == -1) {
        handle_error("epoll_ctl: sockfd");
    }

    for (;;) {
        nfds = epoll_wait(epollfd, events, MAX_EVENTS, -1);
        if (nfds == -1) {
            handle_error("epoll_wait");
        }

        for (int n = 0; n < nfds; ++n) {
            if (events[n].data.fd == sockfd) {
                uint64_t u;
                printf("event found\n");
            }
        }
        fflush(stdout);
    }

    close(sockfd);
}
