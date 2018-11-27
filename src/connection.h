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

typedef struct ClientList {
    Client *array;
    int size;
    int capacity;
} ClientList;

ClientList new_clientlist(int capacity);

void insert_clientlist(ClientList *list, int index, Client item);

void append_clientlist(ClientList *list, Client item);

void clear_clientlist(ClientList *list);

Client remove_clientlist(ClientList *list, int index);

Client pop_clientlist(ClientList *list);

Client index_clientlist(ClientList *list, int index);

void free_clientlist(ClientList *list);
#endif
