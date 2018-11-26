#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <poll.h>
#include "connection.h"
#include "parser.h"
#define BUFF_SIZE 1024

int main(int argc, char const *argv[]) {
    /*Iniciar el server*/
    int port = atoi(argv[1]);
    char const *fdir = argv[2];
    chdir(fdir);
    int listenfd = get_listen_fd(port);
    char buf[BUFF_SIZE];
    int clientslen = 0;
    Client clients[100];
    /*Fin de la inicialización*/

    while(1) {
      /*Iniciar conjunto del Poll*/
      struct pollfd *fds = malloc((clientslen + 1) * sizeof(struct pollfd));
      fds[0].fd = listenfd;
      fds[0].events = POLLIN;
      fds[0].revents = 0;
      for(int i = 0; i < clientslen; ++i) {
        fds[i+1].fd = clients[i].fd;
        fds[i+1].events = POLLIN | POLLOUT;
        fds[i+1].revents = 0;
      }
      /*Ejecutando poll*/
      poll(fds, clientslen + 1, -1);
      /*Ver nuevas conexiones*/
      if(fds[0].revents & POLLIN) {
        Client newclient = get_client_fd(listenfd);
        clients[clientslen++] = newclient;
      }
      for(int i=0; i < clientslen; ++i) {
        if((fds[i+1].revents & POLLIN) && (fds[i+1].revents & POLLOUT)) {
          bzero(buf, BUFF_SIZE);
          read(fds[i+1].fd, buf, BUFF_SIZE);
          Request request = parse_request(buf);
          printf("Petición a la url: %s\n", request.url);
          write(fds[i+1].fd, "HELLO FROM THE OTHER SIDE!", 26);
          close(fds[i+1].fd);
          clients[i].fd = -1;
        }
      }
      /*Liberar conjunto viejo*/
      free(fds);
    }
    /*Fin de Conexión*/  
    close(listenfd);
    return 0;
}
