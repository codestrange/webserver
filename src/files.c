#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <fcntl.h>
#include <wchar.h>
#include <time.h>
#include <sys/sysmacros.h>
#include "files.h"
#include "list.h"
#include "directory.h"

void get_table(CharList *charList, Directory *directory) {
    char *template_row = "<tr><td><a href=\"%s\">%s</a></td><td>%s</td><td>%s</td></tr>%c";
    int len_row = strlen(template_row);
    int len_name = strlen(directory->name);
    int len_path = strlen(directory->path);
    int len_size = strlen(directory->size);
    int len_modified = strlen(directory->modified);
    char *row = malloc(sizeof(char) * (len_row + len_name + len_path + len_size + len_modified + 5));
    bzero(row, len_row + len_name + len_path + len_size + len_modified + 5);
    sprintf(row, template_row, directory->path, directory->name, directory->size, directory->modified, '\0');
    char *temp = row;
    while (*temp) {
        append_charlist(charList, *temp);
        ++temp;
    }
    free(row);
}

void get_template(char *dir_template) {
    int fd = open(dir_template, O_RDONLY, 0664);
    char buffer;
    CharList charList = new_charlist(1000);
    while(read(fd, &buffer, 1)) {
        append_charlist(&charList, buffer);
    }
    template = convert_arraychar(&charList);
    free_charlist(&charList);
}

char *get_html(char *dir, char *url) {
    DirectoryList directories = new_directorylist(100);
    get_directories(dir, url, &directories);
    CharList charList = new_charlist(100);
    for (int i = 0; i < directories.size; ++i) {
        Directory directory = index_directorylist(&directories, i);
        get_table(&charList, &directory);
    }
    free_directorylist(&directories);
    char *table = convert_arraychar(&charList);
    free_charlist(&charList);
    int len = strlen(template) + strlen(table) + 100;
    char *html = malloc(len * sizeof(char));
    sprintf(html, template, table);
    free(table);
    html[len] = 0;
    return html;
}

char *get_response(char *dir, char *url) {
    char *html = get_html(dir, url);
    char *response = malloc((strlen(html) + 100) * sizeof(char));
    bzero(response, strlen(html) + 100);
    sprintf(response, "HTTP/1.1 200 OK\nAccept-Ranges: bytes\nContent-Type: text/html;\nContent-Length: %ld\n\n%s%c", strlen(html), html, '\0');
    free(html);
    return response;
}
