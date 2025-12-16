#ifndef FILE_UTILITIES
#define FILE_UTILITIES

#include <stddef.h>
#include <http_utilities.h>

void http_send_file(http_request_t *request, int client_fd, char *filepath);

#endif
