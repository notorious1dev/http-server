#ifndef FILE_UTILITIES
#define FILE_UTILITIES

#include <stddef.h>
#include <dummy_parser.h>

size_t file_size(char *filepath);
void http_send_file(HttpRequest request, int client_fd, char *filepath);

#endif
