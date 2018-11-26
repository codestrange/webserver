#ifndef LIST
#define LIST

typedef struct CharList {
    char *array;
    int size;
    int capacity;
} CharList;

typedef struct CharCharList {
    CharList *array;
    int size;
    int capacity;
} CharCharList;

CharList new_charlist(int capacity);

void insert_charlist(CharList *list, int index, char item);

void append_charlist(CharList *list, char item);

void clear_charlist(CharList *list);

char remove_charlist(CharList *list, int index);

char pop_charlist(CharList *list);

char index_charlist(CharList *list, int index);

void free_charlist(CharList *list);

CharCharList new_charcharlist(int capacity);

void insert_charcharlist(CharCharList *list, int index, CharList item);

void append_charcharlist(CharCharList *list, CharList item);

void clear_charcharlist(CharCharList *list);

CharList remove_charcharlist(CharCharList *list, int index);

CharList pop_charcharlist(CharCharList *list);

CharList index_charcharlist(CharCharList *list, int index);

void free_charcharlist(CharCharList *list);

char *convert_arraychar(CharList *charList);

char **convert_arraycharchar(CharCharList *charCharList);

CharList convert_CharList(char *str);

CharCharList convert_CharCharList(char **str);

#endif