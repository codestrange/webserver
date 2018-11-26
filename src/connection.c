#include "connection.h"

int get_listen_fd(int port) {
    int listenfd;
    struct sockaddr_in serveraddr;

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    int optval = 1;
    setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, (const void *)&optval, sizeof(int));
    bzero((char *) &serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons((unsigned short)port);

    bind(listenfd, (struct sockaddr *) &serveraddr, sizeof(serveraddr));

    listen(listenfd, 1024);

    return listenfd;
}

client_info get_client_fd(int listenfd) {
    client_info c;
    c.clientlen = sizeof(c.clientaddr);
    c.fd = accept(listenfd, (struct sockaddr *)&c.clientaddr, &c.clientlen);
    return c;
}