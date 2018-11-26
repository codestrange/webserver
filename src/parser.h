#ifndef PARSER
#define PARSER

typedef struct Request {
    char *url;
} Request;

Request parse_request(char *arg);

char **str_split(char *str, const char delim);

#endif