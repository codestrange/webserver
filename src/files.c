#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "files.h"

char *get_html(char *path, char *url) {
    return "<html><body><h1>Hello World!</h1></body></html>";
}

char *get_response(char *dir, char *url) {
    char *html = get_html(dir, url);
    char *response = malloc((strlen(html) + 100) * sizeof(char));
    bzero(response, strlen(html) + 100);
    sprintf(response, "HTTP/1.1 200 OK\nAccept-Ranges: bytes\nContent-Type: text/html;\nContent-Length: %ld\n\n%s", strlen(html), html);
    return response;
}