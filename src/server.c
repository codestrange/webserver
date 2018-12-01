#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <poll.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "connection.h"
#include "parser.h"
#include "files.h"
#define BUFF_SIZE 2048

int main(int argc, char const *argv[]) {
    /*Iniciar el server*/
    int port = atoi(argv[1]);
    char const *fdir = argv[2];
    char *bdir = malloc(BUFF_SIZE * sizeof(char));
    bzero(bdir, BUFF_SIZE);
    getcwd(bdir, BUFF_SIZE);
    sprintf(bdir + strlen(bdir), "%s%c", "/index.html", '\0');
    get_template(bdir);
    free(bdir);
    chdir(fdir);
    int listenfd = get_listen_fd(port);
    char buf[BUFF_SIZE];
    ClientList clients = new_clientlist(1024);
    FileStatusList filets = new_filestatuslist(1024);
    /*Fin de la inicialización*/
    while (true) {
        /*Iniciar conjunto del Poll*/
        bool *ckill = malloc((clients.size + 2) * sizeof(bool));
        for( int i = 0; i < clients.size; ++i) {
            ckill[i] = false;
        }
        struct pollfd *fds = malloc((clients.size + 2) * sizeof(struct pollfd));
        fds[0].fd = listenfd;
        fds[0].events = POLLIN;
        fds[0].revents = 0;
        for(int i = 0; i < clients.size; ++i) {
            fds[i + 1].fd = index_clientlist(&clients, i).fd;
            fds[i + 1].events = POLLIN | POLLOUT;
            fds[i + 1].revents = 0;
        }
        int n = clients.size;
        /*Ejecutando poll*/
        poll(fds, clients.size + 1, -1);
        /*Ver nuevas conexiones*/
        if (fds[0].revents & POLLIN) {
            Client newclient = get_client_fd(listenfd);
            append_clientlist(&clients, newclient);
            FileStatus newfile;
            newfile.fd = -1;
            append_filestatuslist(&filets, newfile);
        }
        for (int i = 0; i < n; ++i) {
            FileStatus check = index_filestatuslist(&filets, i);
            if ( (fds[i + 1].revents & POLLOUT) && check.fd != -1) {
                FileStatus fs = index_filestatuslist(&filets, i);
                remove_filestatuslist(&filets, i);
                send_file_response(&fs, fds[i + 1].fd);
                insert_filestatuslist(&filets, i, fs);
                if ( fs.fd == -1 ) {
                    free(fs.fname);
                    ckill[i] = true;
                }
            }
            else if ((fds[i + 1].revents & POLLIN) && (fds[i + 1].revents & POLLOUT)) {
                bzero(buf, BUFF_SIZE);
                if (read(fds[i + 1].fd, buf, BUFF_SIZE) > 0) {
                    Request request = parse_request(buf);
                    printf("Petición a la url: %s\n", request.url);
                    char *dir = malloc(BUFF_SIZE * sizeof(char));
                    getcwd(dir, BUFF_SIZE);
                    if ( is_folder(dir, request.url) ) {
                        char *response = get_response(dir, request.url);
                        write(fds[i + 1].fd, response, strlen(response));
                        free(response);
                        close(fds[i + 1].fd);
                        ckill[i] = true;
                    }
                    else if ( is_file(dir, request.url) ) {
                        char *path = malloc((strlen(dir) + strlen(request.url) + 5) * sizeof(char));
                        sprintf(path, "%s%s", dir, request.url);
                        FileStatus fs = index_filestatuslist(&filets, i);
                        remove_filestatuslist(&filets, i);
                        fs.fd = open(path, O_RDONLY);
                        fs.fname = get_filename(request.url);
                        fs.size = get_file_size(dir, request.url);
                        fs.offset = 0;
                        send_file_response(&fs, fds[i + 1].fd);
                        insert_filestatuslist(&filets, i, fs);
                        if ( fs.fd == -1 ) {
                            free(fs.fname);
                            ckill[i] = true;
                        }
                    }
                    else {
                        printf("Error 404.\n");
                        close(fds[i + 1].fd);
                        ckill[i] = true;
                    }
                }             
            }
        }
        /*Liberar conjunto viejo*/
        int d = 0;
        for (int i = 0; i < n - d; ++i) {
            if ( ckill[i] ) {
                remove_clientlist(&clients, i);
                remove_filestatuslist(&filets, i);
                --i;
                ++d;
            }
        }
        free(fds);
        free(ckill);
    }
    /*Fin de Conexión*/  
    close(listenfd);
    return 0;
}