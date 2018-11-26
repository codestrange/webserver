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

typedef struct Client {
    int fd;
    int clientlen;
    struct sockaddr_in clientaddr;
} Client;

int get_listen_fd(int port);

Client get_client_fd(int listenfd);
#endif