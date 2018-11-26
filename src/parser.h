#ifndef PARSER
#define PARSER

typedef struct Request {
    char *url;
} Request;

Request parse_request(char *arg);

#endif