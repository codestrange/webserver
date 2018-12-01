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

Client get_client_fd(int listenfd) {
    Client c;
    c.clientlen = sizeof(c.clientaddr);
    c.fd = accept(listenfd, (struct sockaddr *)&c.clientaddr, &c.clientlen);
    return c;
}

ClientList new_clientlist(int capacity) {
    ClientList list;
    list.capacity = capacity;
    list.array = malloc(capacity * sizeof(Client));
    list.size = 0;
    return list;
}

void insert_clientlist(ClientList *list, int index, Client item) {
    if (index < 0)
        index = 0;
    if (index > list->size)
        index = list->size;
    if (list->size == list->capacity) {
        list->capacity *= 2;
        list->array = realloc(list->array, list->capacity * sizeof(Client));
    }
    for (int i = list->size; i > index; --i)
        list->array[i] = list->array[i - 1];
    list->array[index] = item;
    ++list->size;
}

void append_clientlist(ClientList *list, Client item) {
    insert_clientlist(list, list->size, item);
}

void clear_clientlist(ClientList *list) {
    list->size = 0;
}

Client remove_clientlist(ClientList *list, int index) {
    if (index < 0)
        index = 0;
    if (index > list->size)
        index = list->size;
    Client item = list->array[index];
    for (int i = index; i < list->size - 1; ++i)
        list->array[i] = list->array[i + 1];
    --list->size;
    return item;
}

Client pop_clientlist(ClientList *list) {
    return remove_clientlist(list, list->size - 1);
}

Client index_clientlist(ClientList *list, int index) {
    if (index < 0)
        index = 0;
    if (index > list->size)
        index = list->size;
    return list->array[index];
}

void free_clientlist(ClientList *list) {
    free(list->array);
}