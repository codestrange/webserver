#include "directory.h"
#include "utils.h"

void change_order(int new_order) {
    selected_order = new_order;
}

int str_compare(char *left, char *right) {
    while (*left && *right) {
        if (*left < *right)
            return -1;
        else if(*left > *right)
            return 1;
        ++left;
        ++right;
    }
    if (!*left && !*right)
        return 0;
    else if (!*left)
        return -1;
    else
        return 1;
}

int compare(Directory *left, Directory *right) {
    return str_compare(left->name, right->name);
}

void merge(DirectoryList *directories, int left, int middle, int right) {
    DirectoryList result = new_directorylist(10);
    int i = left, j = middle + 1;
    Directory d_left, d_right;
    while (i < middle + 1 && j < right + 1) {
        d_left = index_directorylist(directories, i);
        d_right = index_directorylist(directories, j);
        int cmp = compare(&d_left, &d_right);
        if (cmp <= 0) {
            append_directorylist(&result, d_left);
            ++i;
        } else {
            append_directorylist(&result, d_right);
            ++j;
        }
    }
    while (i < middle + 1) {
        d_left = index_directorylist(directories, i++);
        append_directorylist(&result, d_left);
    }
    while (j < right + 1) {
        d_right = index_directorylist(directories, j++);
        append_directorylist(&result, d_right);
    }
    for (int k = 0; k < result.size; ++k) {
        remove_directorylist(directories, left);
    }
    for (int k = 0; k < result.size; ++k) {
        Directory temp = index_directorylist(&result, k);
        insert_directorylist(directories, k + left, temp);
    }
}

void merge_sort(DirectoryList *directories, int left, int right) {
    if (left >= right)
        return;
    int middle = (left + right) / 2;
    merge_sort(directories, left, middle);
    merge_sort(directories, middle + 1, right);
    merge(directories, left, middle, right);
}