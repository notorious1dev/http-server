#ifndef HTTP_UTILITIES 
#define HTTP_UTILITIES
#include <stddef.h>
#include <sys/types.h>

typedef enum {
    OK = 200,
    NOT_FOUND = 404,
    NOT_IMPLEMENTED = 501,
} HttpStatus;

typedef struct {
    HttpStatus status;
    char *method;
    char *path;

} http_request_t;

typedef enum HTTP_PARSE_STATUS {
    PARSE_ERROR,
    PARSE_SUCCESS,
} HTTP_PARSE_STATUS;

char *status_enum_to_string(HttpStatus);
static ssize_t find_char(char*, ssize_t, ssize_t, char);
int http_parse_request(http_request_t*, char*, size_t);

#endif