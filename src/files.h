#ifndef FILES
#define FILES

#include <sys/sendfile.h>
#include <stdbool.h>
#include "directory.h"
#include "list.h"

typedef struct FileStatus {
    int fd;
    off_t *offset;
} FileStatus;

typedef struct FileStatusList {
    FileStatus *array;
    int size;
    int capacity;
} FileStatusList;

FileStatusList new_filestatuslist(int capacity);

void insert_filestatuslist(FileStatusList *list, int index, FileStatus item);

void append_filestatuslist(FileStatusList *list, FileStatus item);

void clear_filestatuslist(FileStatusList *list);

FileStatus remove_filestatuslist(FileStatusList *list, int index);

FileStatus pop_filestatuslist(FileStatusList *list);

FileStatus index_filestatuslist(FileStatusList *list, int index);

void free_filestatuslist(FileStatusList *list);

char *template;

bool check_mode(char *dir, char *url, int flag);

bool is_file(char *dir, char *url);

bool is_folder(char *dir, char *url);

void get_table(CharList *charList, Directory *directory);

void get_template(char *base_dir);

char *get_html(char *path, char *url);

char *get_response(char *dir, char *url);

#endif