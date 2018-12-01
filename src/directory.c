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
#include "directory.h"
#include "list.h"
#include "utils.h"

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
    for (int i = list->size; i > index; --i)
        list->array[i] = list->array[i - 1];
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
    free(list->array);
}

int count_digits(int number) {
    int cont = 0;
    while (number) {
        number /= 10;
        ++cont;
    }
    return cont;
}

bool get_directory(char *current_dir, char *path, char *filename, Directory *directory) {
    char *temp_current_dir = current_dir;
    char *temp_path = path;
    char *temp_filename = filename;
    CharList charList = new_charlist(100);
    while (*temp_current_dir) {
        append_charlist(&charList, *temp_current_dir);
        ++temp_current_dir;
    }
    while (*temp_path) {
        append_charlist(&charList, *temp_path);
        ++temp_path;
    }
    while (*temp_filename) {
        append_charlist(&charList, *temp_filename);
        ++temp_filename;
    }
    char *full_path = convert_arraychar(&charList);
    free_charlist(&charList);
    struct stat info;
    if (stat(full_path, &info) < 0)
        return false;
    char *_temp_modified = ctime(&info.st_mtime);
    CharList _path_list = convert_CharList(path);
    CharList _name_list = convert_CharList(filename);
    CharList _modified_list = convert_CharList(_temp_modified);
    for (int i = 0; i < _name_list.size; ++i) {
        char c = index_charlist(&_name_list, i);
        append_charlist(&_path_list, c);
    }
    if (info.st_mode & __S_IFDIR)
        append_charlist(&_path_list, '/');
    char *_path = convert_arraychar(&_path_list);
    char *_name = convert_arraychar(&_name_list);
    char *_modified = convert_arraychar(&_modified_list);
    _modified[_modified_list.size - 1] = '\0';
    int digits = count_digits(info.st_size) + 10;
    char *_size;
    if (info.st_mode & __S_IFDIR) {
        _size = malloc(sizeof(char) * 10);    
        bzero(_size, 10);
        sprintf(_size, "---%c", '\0');
    } else {
        _size = malloc(sizeof(char) * digits);
        bzero(_size, digits);
        sprintf(_size, "%ld Bytes%c", info.st_size, '\0');
    }
    free_charlist(&_path_list);
    free_charlist(&_name_list);
    free_charlist(&_modified_list);
    directory->path = _path;
    directory->name = _name;
    directory->modified = _modified;
    directory->size = _size;
    directory->_size = info.st_size;
    directory->_modified = info.st_mtime;
    directory->is_file = info.st_mode & __S_IFREG;
    return true;
}

bool get_directories(char *current_dir, char *path, DirectoryList *directoryList) {
    Directory first;
    Directory second;
    char *temp_current_dir = current_dir;
    char *temp_path = path;
    CharList charList = new_charlist(100);
    while (*temp_current_dir) {
        append_charlist(&charList, *temp_current_dir);
        ++temp_current_dir;
    }
    while (*temp_path) {
        append_charlist(&charList, *temp_path);
        ++temp_path;
    }
    char *full_path = convert_arraychar(&charList);
    free_charlist(&charList);
    DIR *dir = opendir(full_path);
    if (dir == NULL)
        return false;
    while (true) {
        struct dirent *d = readdir(dir);
        if (d == NULL)
            break;
        Directory directory;
        if (!get_directory(current_dir, path, d->d_name, &directory))
            return false;
        if (!strncmp(directory.name, ".", 1) && strlen(directory.name) == 1)
            first = directory;
        else if (!strncmp(directory.name, "..", 2) && strlen(directory.name) == 2)
            second = directory;
        else
            append_directorylist(directoryList, directory);
    }
    free(full_path);
    closedir(dir);
    merge_sort(directoryList, 0, directoryList->size - 1);
    insert_directorylist(directoryList, 0, second);
    insert_directorylist(directoryList, 0, first);
    return true;
}