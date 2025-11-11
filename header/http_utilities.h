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

} HttpRequest;

//helping functions
char *status_enum_to_string(HttpStatus status);
static ssize_t find_char(char* buffer, ssize_t offset, ssize_t length, char c);

HttpRequest *http_parse_request(char* buffer, size_t length);

#endif