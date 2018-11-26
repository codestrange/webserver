#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "list.h"

CharList new_charlist(int capacity) {
    CharList list;
    list.capacity = capacity;
    list.array = malloc(capacity * sizeof(char));
    list.size = 0;
    return list;
}

void insert_charlist(CharList *list, int index, char item) {
    if (index < 0)
        index = 0;
    if (index > list->size)
        index = list->size;
    if (list->size == list->capacity) {
        list->capacity *= 2;
        list->array = realloc(list->array, list->capacity * sizeof(char));
    }
    for (int i = index; i < list->size; ++i)
        list->array[i + 1] = list->array[i];
    list->array[index] = item;
    ++list->size;
}

void append_charlist(CharList *list, char item) {
    insert_charlist(list, list->size, item);
}

void clear_charlist(CharList *list) {
    list->size = 0;
}

char remove_charlist(CharList *list, int index) {
    if (index < 0)
        index = 0;
    if (index > list->size)
        index = list->size;
    char item = list->array[index];
    for (int i = index; i < list->size - 1; ++i)
        list->array[i] = list->array[i + 1];
    --list->size;
    return item;
}

char pop_charlist(CharList *list) {
    return remove_charlist(list, list->size - 1);
}

char index_charlist(CharList *list, int index) {
    if (index < 0)
        index = 0;
    if (index > list->size)
        index = list->size;
    return list->array[index];
}

void free_charList(CharList *list) {
    free(list->array);
}

CharCharList new_charcharlist(int capacity) {
    CharCharList list;
    list.capacity = capacity;
    list.array = malloc(capacity * sizeof(CharList));
    list.size = 0;
    return list;
}

void insert_charcharlist(CharCharList *list, int index, CharList item) {
    if (index < 0)
        index = 0;
    if (index > list->size)
        index = list->size;
    if (list->size == list->capacity) {
        list->capacity *= 2;
        list->array = realloc(list->array, list->capacity * sizeof(CharList));
    }
    for (int i = index; i < list->size; ++i)
        list->array[i + 1] = list->array[i];
    list->array[index] = item;
    ++list->size;
}

void append_charcharlist(CharCharList *list, CharList item) {
    insert_charcharlist(list, list->size, item);
}

void clear_charcharlist(CharCharList *list) {
    list->size = 0;
}

CharList remove_charcharlist(CharCharList *list, int index) {
    if (index < 0)
        index = 0;
    if (index > list->size)
        index = list->size;
    CharList item = list->array[index];
    for (int i = index; i < list->size - 1; ++i)
        list->array[i] = list->array[i + 1];
    --list->size;
    return item;
}

CharList pop_charcharlist(CharCharList *list) {
    return remove_charcharlist(list, list->size - 1);
}

CharList index_charcharlist(CharCharList *list, int index) {
    if (index < 0)
        index = 0;
    if (index > list->size)
        index = list->size;
    return list->array[index];
}

void free_charcharList(CharCharList *list) {
    CharList *charList = list->array;
    for (int i = 0; i < list->size; ++i) {
        free_charList(charList++);
    }
}

char *convert_arraychar(CharList *charList) {
    char *result = malloc((charList->size + 1) * sizeof(char));
    for (int i = 0; i < charList->size; ++i) {
        result[i] = index_charlist(charList, i);
    }
    result[charList->size] = 0;
    return result;
}

char **convert_arraycharchar(CharCharList *charCharList) {
    char **result = malloc((charCharList->size + 1) * sizeof(char*));
    CharList charList;
    for (int i = 0; i < charCharList->size; ++i) {
        charList = index_charcharlist(charCharList, i);
        result[i] = convert_arraychar(&charList);
    }
    result[charCharList->size] = 0;
    return result;
}

CharList convert_CharList(char *str) {
    CharList charList = new_charlist(10);
    while(*str) {
        append_charlist(&charList, *str);
        ++str;
    }
    return charList;
}

CharCharList convert_CharCharList(char **str) {
    CharCharList charCharList = new_charcharlist(10);
    while(*str) {
        CharList charList = convert_CharList(*str);
        append_charcharlist(&charCharList, charList);
        ++str;
    }
    return charCharList;
}