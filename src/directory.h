#ifndef DIRECTORY
#define DIRECTORY

#include <stdbool.h>

typedef struct Directory {
    char *path;
    char *name;
    char *size;
    char *modified;
} Directory;

typedef struct DirectoryList {
    Directory *array;
    int size;
    int capacity;
} DirectoryList;

DirectoryList new_directorylist(int capacity);

void insert_directorylist(DirectoryList *list, int index, Directory item);

void append_directorylist(DirectoryList *list, Directory item);

void clear_directorylist(DirectoryList *list);

Directory remove_directorylist(DirectoryList *list, int index);

Directory pop_directorylist(DirectoryList *list);

Directory index_directorylist(DirectoryList *list, int index);

void free_directorylist(DirectoryList *list);

int count_digits(int number);

bool get_directory(char *current_dir, char *path, char *filename, Directory *directory);

bool get_directories(char *current_dir, char *path, DirectoryList *directoryList);

#endif