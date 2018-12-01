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

FileStatusList new_filestatuslist(int capacity) {
    FileStatusList list;
    list.capacity = capacity;
    list.array = malloc(capacity * sizeof(FileStatus));
    list.size = 0;
    return list;
}

void insert_filestatuslist(FileStatusList *list, int index, FileStatus item) {
    if (index < 0)
        index = 0;
    if (index > list->size)
        index = list->size;
    if (list->size == list->capacity) {
        list->capacity *= 2;
        list->array = realloc(list->array, list->capacity * sizeof(FileStatus));
    }
    for (int i = index; i < list->size; ++i)
        list->array[i + 1] = list->array[i];
    list->array[index] = item;
    ++list->size;
}

void append_filestatuslist(FileStatusList *list, FileStatus item) {
    insert_filestatuslist(list, list->size, item);
}

void clear_filestatuslist(FileStatusList *list) {
    list->size = 0;
}

FileStatus remove_filestatuslist(FileStatusList *list, int index) {
    if (index < 0)
        index = 0;
    if (index > list->size)
        index = list->size;
    FileStatus item = list->array[index];
    for (int i = index; i < list->size - 1; ++i)
        list->array[i] = list->array[i + 1];
    --list->size;
    return item;
}

FileStatus pop_filestatuslist(FileStatusList *list) {
    return remove_filestatuslist(list, list->size - 1);
}

FileStatus index_filestatuslist(FileStatusList *list, int index) {
    if (index < 0)
        index = 0;
    if (index > list->size)
        index = list->size;
    return list->array[index];
}

void free_filestatuslist(FileStatusList *list) {
    free(list->array);
}

long get_file_size(char *dir, char *url) {
    char *temp_dir = dir;
    char *temp_url = url;
    CharList charList = new_charlist(100);
    while (*temp_dir) {
        append_charlist(&charList, *temp_dir);
        ++temp_dir;
    }
    while (*temp_url) {
        append_charlist(&charList, *temp_url);
        ++temp_url;
    }
    char *full_path = convert_arraychar(&charList);
    free_charlist(&charList);
    struct stat info;
    stat(full_path, &info);
    free(full_path);
    return info.st_size;
}

bool check_mode(char *dir, char *url, int flag) {
    char *temp_dir = dir;
    char *temp_url = url;
    CharList charList = new_charlist(100);
    while (*temp_dir) {
        append_charlist(&charList, *temp_dir);
        ++temp_dir;
    }
    while (*temp_url) {
        append_charlist(&charList, *temp_url);
        ++temp_url;
    }
    char *full_path = convert_arraychar(&charList);
    free_charlist(&charList);
    struct stat info;
    stat(full_path, &info);
    free(full_path);
    return info.st_mode & flag;
}

bool is_file(char *dir, char *url) {
    return check_mode(dir, url, __S_IFREG);
}

bool is_folder(char *dir, char *url) {
    return check_mode(dir, url, __S_IFDIR);
}

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
    char *html = malloc((len + 1) * sizeof(char));
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

void send_file_response(FileStatus *file, int conectionfd) {
    if( file->offset == 0 ) {
        char *header = malloc((250 + strlen(file->fname)) * sizeof(char));
        sprintf(header,"HTTP/1.1 200 OK\nAccept-Ranges: bytes\nCache-Control: must-revalidate\nContent-Description: File Transfer\nContent-Disposition: attachment; filename=%s\nContent-Length: %ld\nContent-Transfer-Encoding: binary\nExpires: 0\n\n", file->fname, file->size);
        write(conectionfd, header, strlen(header));
        free(header);
    }

    int status = sendfile(conectionfd, file->fd, &(file->offset), 1024);
    if( status < 0 ) {
        perror("Error sending file");
        close(conectionfd);
        close(file->fd);
        file->fd = -1;
    }
    else if( status < 1024 ){ 
        close(conectionfd);
        close(file->fd);
        file->fd = -1;
    }
}
