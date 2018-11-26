#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "connection.h"
#define BUFF_SIZE 1024

void aux_write(int fd, char* buf, int size)
{
  int left = size;
  while(1)
  {
     int n = write(fd, buf, left);
     left-=n;
     if(n<=0 || left<=0) break; 
  }  
}

int main(int argc, char const *argv[]) {
    /*Iniciar el server*/
    int port = atoi(argv[1]);
    char const *fdir = argv[2];
    chdir(fdir);
    int listenfd = get_listen_fd(port);
    char buf[BUFF_SIZE];
    bzero(buf, BUFF_SIZE);
    /*Fin de la inicialización*/
    /*Estableciendo conexión*/
    client_info client = get_client_fd(listenfd);
    if (client.fd < 0) {
        printf("ERROR on connection.\n");
    }
    else if (read(client.fd, buf, BUFF_SIZE)) {
        aux_write(1, buf, BUFF_SIZE);
    }
    /*Fin de Conexión*/
    close(client.fd);    
    close(listenfd);
    return 0;
}
