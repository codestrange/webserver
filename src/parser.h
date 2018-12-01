#ifndef PARSER
#define PARSER

typedef struct Request {
    char *url;
    char *param;
} Request;

bool is_hex(char c);

int hex2dec(char c);

char *decode(char *str);

char *get_filename(char *url);

Request parse_request(char *arg);

char **str_split(char *str, const char delim);

#endif