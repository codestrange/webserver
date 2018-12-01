#ifndef PARSER
#define PARSER

typedef struct Request {
    char *url;
} Request;

char *get_filename(char *url);

Request parse_request(char *arg);

char **str_split(char *str, const char delim);

#endif