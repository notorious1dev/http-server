#include <string.h>
#include <dummy_parser.h>
#include <stdlib.h>

HttpRequest http_parse_request(char* buffor, size_t lenght) {
    HttpRequest request = {0};

    //parse method
    int space_after_method = strchr(buffor, ' ') - buffor; //first space index after method
    char *mt = malloc(space_after_method + 1); // +1 for \0
    for (int i = 0; i < space_after_method ; i++) { mt[i] = buffor[i] ;}
    mt[space_after_method] = '\0';
    request.method = mt;

    //parse path
    int space_after_path = strchr(buffor + space_after_method + 1, ' ') - buffor;
    char *pt = malloc(space_after_path - space_after_method + 1);
    for (int i = space_after_method + 1; i < space_after_path ; i++) { pt[i - (space_after_method + 1)] = buffor[i] ;}
    pt[space_after_path - space_after_method - 1] = '\0';
    request.path = pt;

    return request;
}