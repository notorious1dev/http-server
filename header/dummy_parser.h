#ifndef DUMMY_PARSER
#define DUMMY_PARSER
#include <stddef.h>

typedef enum {
    OK = 200,
    NOT_FOUND = 404,
    NOT_IMPLEMENTED = 501,
} HttpStatus;

typedef struct {
    char *method;
    char *path;

} HttpRequest;

HttpRequest http_parse_request(char* buffer);

#endif