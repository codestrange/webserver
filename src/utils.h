#ifndef UTILS
#define UTILS

#include "directory.h"

int selected_order;

void change_order(int new_order);

int str_compare(char *left, char *right);

int compare(Directory *left, Directory *right);

void merge(DirectoryList *directories, int left, int middle, int right);

void merge_sort(DirectoryList *directories, int left, int right);

#endif
