#ifndef CONNECTION
#define CONNECTION
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/select.h>

typedef struct client_info {
    int fd;
    int clientlen;
    struct sockaddr_in clientaddr;
} client_info;

int get_listen_fd(int port);

client_info get_client_fd(int listenfd);
#endif