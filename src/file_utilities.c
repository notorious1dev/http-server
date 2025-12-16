#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/sendfile.h>
#include "http_utilities.h"

void http_send_file(http_request_t *request, int client_fd, char *filepath) {
        int fd = open(filepath, O_RDONLY);
        struct stat file_stat;
        if(fstat(fd, &file_stat) == -1) {
            printf("Error with file_stat\n");
            goto clean_up;
        }

        if (fd == -1) {
            printf("Error opening %s\n", filepath);
            goto clean_up;
        }

        char header[512];
        char *status = status_enum_to_string(request->status);
        int header_len = snprintf(header, sizeof(header),
            "HTTP/1.1 %d %s\r\n"
            "Content-Type: text/html\r\n"
            "Content-Length: %zu\r\n"
            "Connection: close\r\n"
            "\r\n",request->status, status, file_stat.st_size);

        if (send(client_fd, header, header_len, 0) < 0) {
            printf("Error: Header send failed");
            goto clean_up;
        }

        if (sendfile(client_fd, fd, NULL, file_stat.st_size) < 0) {
            printf("Error: File send failed");
            goto clean_up;
        }

        clean_up:
        close(fd);
        return;
}