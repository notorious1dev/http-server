#include <stddef.h>
#ifndef DUMMY_PARSER
#define DUMMY_PARSER

typedef enum {
    OK = 200,
    NOT_FOUND = 404,
} HttpStatus;

typedef struct {
    char *method;
    char *path;

} HttpRequest;

HttpRequest http_parse_request(char* buffor, size_t lenght);

#endif