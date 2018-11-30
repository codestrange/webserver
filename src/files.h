#ifndef FILES
#define FILES

#include "directory.h"
#include "list.h"

char *template;

void get_table(CharList *charList, Directory *directory);

void get_template(char *base_dir);

char *get_html(char *path, char *url);

char *get_response(char *dir, char *url);

#endif
