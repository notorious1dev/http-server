#include <string.h>
#include <dummy_parser.h>
#include <stdlib.h>


static ssize_t find_char(char* buffer, ssize_t offset, ssize_t length, char c)
{
    if (buffer == NULL)
        return (ssize_t)-1;

    if (offset >= length)
        return (ssize_t)-1;

    for (ssize_t i = offset; i < length; i ++)
    {
        if (buffer[i] == c)
            return i;
    }

    return (ssize_t) -1;
}

HttpRequest* http_parse_request(char* buffer, size_t length) {
    HttpRequest *request = malloc(sizeof(HttpRequest));

    //parse method
    size_t space_after_method = find_char(buffer, 0, length, ' '); //first space index after method
    if (space_after_method == -1){
        goto fail;
    }

    char *mt = malloc(space_after_method + 1); // +1 for \0
    for (int i = 0; i < space_after_method ; i++) { mt[i] = buffer[i] ;}
    mt[space_after_method] = '\0';
    request->method = mt;

    //parse path
    size_t space_after_path = find_char(buffer, space_after_method + 1, length, ' ');
    if (space_after_path == -1){
        goto fail;
    }

    char *pt = malloc(space_after_path - space_after_method + 1);
    for (int i = space_after_method + 1; i < space_after_path ; i++) { pt[i - (space_after_method + 1)] = buffer[i] ;}
    pt[space_after_path - space_after_method - 1] = '\0';
    request->path = pt;

    return request;

    fail:
    free(mt);
    free(pt);
    return NULL;
}