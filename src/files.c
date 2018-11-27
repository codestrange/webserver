#include "files.h"

char *get_html(char *path) {
    return "<html><body><h1>Hello World!</h1></body></html>";
}

char *get_response(char *path) {
    char *html = get_html(path);
    char *response = malloc((strlen(html) + 100) * sizeof(char));
    bzero(response, 2048);
    sprintf(response, "HTTP/1.1 200 OK\nAccept-Ranges: bytes\nContent-Type: text/html;\nContent-Length: %ld\n\n%s", strlen(html), html);
    return response;
}