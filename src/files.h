#ifndef FILES
#define FILES

#include <stdbool.h>
#include "directory.h"
#include "list.h"

char *template;

bool check_mode(char *dir, char *url, int flag);

bool is_file(char *dir, char *url);

bool is_folder(char *dir, char *url);

void get_table(CharList *charList, Directory *directory);

void get_template(char *base_dir);

char *get_html(char *path, char *url);

char *get_response(char *dir, char *url);

#endif