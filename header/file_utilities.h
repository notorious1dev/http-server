#ifndef FILE_UTILITIES
#define FILE_UTILITIES

#include <stddef.h>
#include <http_utilities.h>

int find_file(http_request_t *request, char *dir_path, char *out_path, size_t out_size);
void http_send_file(http_request_t *request, int client_fd, char *filepath);

#endif
