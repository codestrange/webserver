#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <poll.h>
#include "connection.h"
#include "parser.h"
#include "files.h"
#define BUFF_SIZE 1024

int main(int argc, char const *argv[]) {
    /*Iniciar el server*/
    int port = atoi(argv[1]);
    char const *fdir = argv[2];
    chdir(fdir);
    int listenfd = get_listen_fd(port);
    char buf[BUFF_SIZE];
    ClientList clients = new_clientlist(1024);
    /*Fin de la inicialización*/

    while(1) {
      /*Iniciar conjunto del Poll*/
      struct pollfd *fds = malloc((clients.size + 1) * sizeof(struct pollfd));
      fds[0].fd = listenfd;
      fds[0].events = POLLIN;
      fds[0].revents = 0;
      for(int i = 0; i < clients.size; ++i) {
        fds[i+1].fd = index_clientlist(&clients, i).fd;
        fds[i+1].events = POLLIN | POLLOUT;
        fds[i+1].revents = 0;
      }
      /*Ejecutando poll*/
      poll(fds, clients.size + 1, -1);
      /*Ver nuevas conexiones*/
      if(fds[0].revents & POLLIN) {
        Client newclient = get_client_fd(listenfd);
        append_clientlist(&clients, newclient);
      }
      for(int i=0; i < clients.size; ++i) {
        if((fds[i+1].revents & POLLIN) && (fds[i+1].revents & POLLOUT)) {
          bzero(buf, BUFF_SIZE);
          if( read(fds[i+1].fd, buf, BUFF_SIZE) > 0) {
            Request request = parse_request(buf);
            printf("Petición a la url: %s\n", request.url);
            char *dir = malloc(BUFF_SIZE * sizeof(char));
            getcwd(dir, BUFF_SIZE);
            sprintf(dir + strlen(dir),"%s",request.url);
            char *response = get_response(dir);
            write(fds[i+1].fd, response, strlen(response));
            free(response); 
          }
          close(fds[i+1].fd);
          remove_clientlist(&clients, i);
        }
      }
      /*Liberar conjunto viejo*/
      free(fds);
    }
    /*Fin de Conexión*/  
    close(listenfd);
    return 0;
}
