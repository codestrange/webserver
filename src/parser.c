#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "parser.h"
#include "list.h"

bool is_hex(char c) {
    return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F');
}

int hex2dec(char c) {
    if (c >= '0' && c <= '9') {
        return c - '0';
    }
    switch (c) {
        case 'a':
        case 'A':
            return 10;
        case 'b':
        case 'B':
            return 11;
        case 'c':
        case 'C':
            return 12;
        case 'd':
        case 'D':
            return 13;
        case 'e':
        case 'E':
            return 14;
        default:
            return 15;
    }
}

char *decode(char *str) {
    CharList charList = convert_CharList(str);
    CharList result = new_charlist(10);
    int i = 0;
    while (i < charList.size) {
        char c = index_charlist(&charList, i);
        if (c == '%' && i + 2 < charList.size) {
            char c1 = index_charlist(&charList, i + 1);
            char c2 = index_charlist(&charList, i + 2);
            if (is_hex(c1) && is_hex(c2)) {
                int d1 = hex2dec(c1);
                int d2 = hex2dec(c2);
                c = (char) (d1 * 16 + d2);
                append_charlist(&result, c);
                i += 3;
                continue;
            }
        }
        append_charlist(&result, c);
        ++i;
    }
    char *str_decoded = convert_arraychar(&result);
    free_charlist(&charList);
    free_charlist(&result);
    return str_decoded;
}

char *get_filename(char *url) {
    char **splited = str_split(url, '/');
    CharCharList charCharList = convert_CharCharList(splited);
    CharList charList = index_charcharlist(&charCharList, charCharList.size - 1);
    char *result = convert_arraychar(&charList);
    return result;
}

Request parse_request(char *arg) {
    char **splited = str_split(arg, ' ');
    CharCharList charCharList = convert_CharCharList(splited);
    Request result;
    CharList charList = index_charcharlist(&charCharList, 1);
    char *temp = convert_arraychar(&charList);
    char *request = decode(temp);
    char **_splited = str_split(request, '?');
    charCharList = convert_CharCharList(_splited);
    CharList charList1 = index_charcharlist(&charCharList, 0);
    char *_url = convert_arraychar(&charList1);
    result.url = _url;
    if (charCharList.size > 1) {
        CharList charList2 = index_charcharlist(&charCharList, 1);
        char *_param = convert_arraychar(&charList2);
        result.param = _param;
    }
    free(temp);
    return result;
}

char **str_split(char *str, const char delim) {
    char **result = 0;
    int count = 0;
    char *tmp = str;
    char *last_comma = 0;
    char _delim[2];
    _delim[0] = delim;
    _delim[1] = 0;
    while (*tmp) {
        if (delim == *tmp) {
            ++count;
            last_comma = tmp;
        }
        ++tmp;
    }
    count += (last_comma < (str + strlen(str) - 1)) + 1;
    result = malloc(sizeof(char *) * (count + 5));
    char *_str = malloc(sizeof(char) * (strlen(str)) + 5);
    char *_tmp = _str;
    tmp = str;
    while (true) {
        *_tmp = *tmp;
        if (!*tmp)
            break;
        ++_tmp;
        ++tmp;
    }
    if (result) {
        int idx = 0;
        char *token = strtok(_str, _delim);
        while (token) {
            *(result + idx++) = strdup(token);
            token = strtok(0, _delim);
        }
        *(result + idx) = 0;
    }
    return result;
}