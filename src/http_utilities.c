#include <string.h>
#include <http_utilities.h>
#include <stdlib.h>

char * status_enum_to_string(HttpStatus status) {
    switch (status)
    {
        case OK: return "OK";
        case NOT_FOUND: return "NOT FOUND";
        case NOT_IMPLEMENTED: return "NOT IMPLEMENTED";
        default: return "NOT IMPLEMENTED";
    }
}

static ssize_t find_char(char* buffer, ssize_t offset, ssize_t length, char c)
{
    if (buffer == NULL)
        return -1;

    if (offset >= length || offset < 0)
        return -1;

    for (ssize_t i = offset; i < length; i ++)
    {
        if (buffer[i] == c)
            return i;
    }

    return -1;
}

int http_parse_request(http_request_t* request, char* buffer, size_t buffer_length) {
    if (buffer == NULL || request == NULL || buffer_length == 0)
        return PARSE_ERROR;

    if (request == NULL)
        return PARSE_ERROR;

    //parse method
    ssize_t method_end = find_char(buffer, 0, buffer_length, ' '); //first space index after method
    if (method_end == -1){
        return PARSE_ERROR;
    }

    request->method = malloc(method_end + 1); // +1 for \0
    if (request->method == NULL) {
        return PARSE_ERROR;
    }

    for (int i = 0; i < method_end ; i++) 
        request->method[i] = buffer[i];

    request->method[method_end] = '\0';

    //parse path
    ssize_t path_end = find_char(buffer, method_end + 1, buffer_length, ' ');
    if (path_end == -1){
        free(request->method);
        free(request);
        return PARSE_ERROR;
    }

    request->path = malloc(path_end - method_end + 1);
    if (request->path == NULL)
    {
        free(request->method);
        free(request);
        return PARSE_ERROR;
    }

    for (int i = method_end + 1; i < path_end ; i++) 
        request->path[i - (method_end + 1)] = buffer[i];

    request->path[path_end - method_end - 1] = '\0';
    request->status = OK;

    return PARSE_SUCCESS;
}