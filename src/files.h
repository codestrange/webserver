#ifndef FILES
#define FILES

#include <sys/sendfile.h>
#include "directory.h"
#include "list.h"

typedef struct File_Status{
    int fd;
    off_t *offset;
} File_Status;

char *template;

void get_table(CharList *charList, Directory *directory);

void get_template(char *base_dir);

char *get_html(char *path, char *url);

char *get_response(char *dir, char *url);

#endif
