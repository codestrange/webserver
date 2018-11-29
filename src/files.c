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

DirectoryList new_directorylist(int capacity) {
    DirectoryList list;
    list.capacity = capacity;
    list.array = malloc(capacity * sizeof(Directory));
    list.size = 0;
    return list;
}

void insert_directorylist(DirectoryList *list, int index, Directory item) {
    if (index < 0)
        index = 0;
    if (index > list->size)
        index = list->size;
    if (list->size == list->capacity) {
        list->capacity *= 2;
        list->array = realloc(list->array, list->capacity * sizeof(Directory));
    }
    for (int i = index; i < list->size; ++i)
        list->array[i + 1] = list->array[i];
    list->array[index] = item;
    ++list->size;
}

void append_directorylist(DirectoryList *list, Directory item) {
    insert_directorylist(list, list->size, item);
}

void clear_directorylist(DirectoryList *list) {
    list->size = 0;
}

Directory remove_directorylist(DirectoryList *list, int index) {
    if (index < 0)
        index = 0;
    if (index > list->size)
        index = list->size;
    Directory item = list->array[index];
    for (int i = index; i < list->size - 1; ++i)
        list->array[i] = list->array[i + 1];
    --list->size;
    return item;
}

Directory pop_directorylist(DirectoryList *list) {
    return remove_directorylist(list, list->size - 1);
}

Directory index_directorylist(DirectoryList *list, int index) {
    if (index < 0)
        index = 0;
    if (index > list->size)
        index = list->size;
    return list->array[index];
}

void free_directorylist(DirectoryList *list) {
    Directory *directory = list->array;
    for (int i = 0; i < list->size; ++i) {
        free(directory->path);
        free(directory->name);
        free(directory->size);
        free(directory->modified);
        ++directory;
    }
}

int count_digits(int number) {
    int cont = 0;
    while (number) {
        number /= 10;
        ++cont;
    }
    return cont;
}

bool get_file(char *current_dir, char *path, char *filename, Directory *directory) {
    size_t len = strlen(current_dir) + strlen(path) + strlen(filename);
    char *buffer = malloc(sizeof(char) * (len + 5));
    bzero(buffer, len + 5);
    sprintf(buffer, "%s%s%s", current_dir, path, filename);
    struct stat info;
    if (stat(buffer, &info) < 0)
        return false;
    len = strlen(path) + strlen(filename);
    // buffer = malloc(sizeof(char) * (len + 1));
    bzero(buffer, len + 5);
    if (info.st_mode & __S_IFDIR)
        sprintf(buffer, "%s%s/", path, filename);
    else 
        sprintf(buffer, "%s%s", path, filename);
    directory->path = buffer;
    len = strlen(filename);
    buffer = malloc(sizeof(char) * (len + 5));
    bzero(buffer, (len + 5));
    sprintf(buffer, "%s", filename);
    directory->name = buffer;
    char *modified = ctime((const time_t *) &info.st_mtime);
    len = strlen(modified);
    buffer = malloc(sizeof(char) * len);
    sprintf(buffer, "%s", modified);
    buffer[len - 1] = 0;
    directory->modified = buffer;
    __off_t size = info.st_size;
    int digits = count_digits((int) size);
    buffer = malloc(sizeof(char) * (digits + 6));
    bzero(buffer, digits + 6);
    sprintf(buffer, "%d Bytes", (int) size);
    directory->size = buffer;
    return true;
}

bool get_files(char *current_dir, char *path, DirectoryList *directoryList) {
    long len_current_dir = strlen(current_dir);
    long len_path = strlen(path);
    char *buffer = malloc(sizeof(char) * (len_current_dir + len_path + 5));
    bzero(buffer, len_current_dir + len_path + 5);
    sprintf(buffer, "%s%s", current_dir, path);
    DIR *dir = opendir(buffer); 
    if (dir == NULL)
        return false;
    while (true) {
        struct dirent *d = readdir(dir);
        if (d == NULL)
            break;
        Directory directory;
        if (!get_file(current_dir, path, d->d_name, &directory))
            return false;
        if(!(!strncmp(directory.name,".",1) && strlen(directory.name) == 1)) {
            append_directorylist(directoryList, directory);
        }
    }
    free(buffer);
    closedir(dir);
    return true;
}

char *get_row(Directory *directory) {
    char *template_row = "<tr><td><a href=\"%s\">%s</a></td><td>%s</td><td>%s</td></tr>";
    int len_row = strlen(template_row);
    int len_name = strlen(directory->name);
    int len_path = strlen(directory->path);
    int len_size = strlen(directory->size);
    int len_modified = strlen(directory->modified);
    char *row = malloc(sizeof(char) * (len_row + len_name + len_path + len_size + len_modified + 5));
    bzero(row, len_row + len_name + len_path + len_size + len_modified + 5);
    sprintf(row, template_row, directory->path, directory->name, directory->size, directory->modified);
    return row;
}

char *get_table(DirectoryList *directoryList) {
    char **rows = malloc(sizeof(char *) * (directoryList->size + 5));
    int *len_rows = malloc(sizeof(int) * (directoryList->size + 5));
    int len = 0;
    for (int i = 0; i < directoryList->size; ++i) {
        Directory directory = index_directorylist(directoryList, i);
        rows[i] = get_row(&directory);
        len_rows[i] = strlen(rows[i]);
        len += len_rows[i];
    }
    char *table = malloc(sizeof(char) * (len + 5));
    bzero(table, len + 5);
    int index = 0;
    for (int i = 0; i < directoryList->size; ++i) {
        for (int j = 0; j < len_rows[i]; ++j) {
            table[index++] = rows[i][j];
        }
        free(rows[i]);
    }
    free(rows);
    table[index] = 0;
    return table;
}

char *get_html(char *current_dir, char *path) {
    int fd = open("index.html", O_RDONLY, 0664);
    int len_template = 0;
    char buffer;
    while (read(fd, &buffer, 1))
        ++len_template;
    close(fd);
    fd = open("index.html", O_RDONLY , 0664);
    char *template = malloc(sizeof(char) * (len_template + 5));
    bzero(template, len_template + 5);
    int index = 0;
    while (read(fd, &buffer, 1))
        template[index++] = buffer;
    template[index] = 0;
    close(fd);
    DirectoryList directoryList = new_directorylist(10);
    get_files(current_dir, path, &directoryList);
    char *table = get_table(&directoryList);
    free_directorylist(&directoryList);
    int len_table = strlen(table);
    char *html = malloc(sizeof(char) * (len_template + len_table + 5));
    bzero(html, len_template + len_table + 5);
    sprintf(html, template, table);
    printf("Silvio\n");
    free(template);
    free(table);
    return html;
}

char *get_response(char *current_dir, char *path) {
    char *html = get_html(current_dir, path);
    long lhtml = strlen(html);
    char *response = malloc((lhtml + 100) * sizeof(char));
    bzero(response, lhtml + 100);
    sprintf(response, "HTTP/1.1 200 OK\nAccept-Ranges: bytes\nContent-Type: text/html;\nContent-Length: %ld\n\n%s%c", lhtml, html, 0);
    free(html);
    return response;
}