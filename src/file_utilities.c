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


int find_file(HttpRequest *request, const char *dir_path, char *out_path, size_t out_size) {
    DIR *dir = opendir(dir_path);
    if (!dir) {
        printf("Error: failed to open directory %s\n", dir_path);
        return 0;
    }

    struct dirent *entry;
    char *filename = (request->path[0] == '/') ? request->path + 1 : request->path;

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        char path[4096];
        snprintf(path, sizeof(path), "%s/%s", dir_path, entry->d_name);

        struct stat st;
        if (stat(path, &st) == -1)
            continue;

        if (S_ISDIR(st.st_mode)) {
            if (find_file(request, path, out_path, out_size)) {
                closedir(dir);
                return 1;
            }
        } else if (S_ISREG(st.st_mode) && strcmp(entry->d_name, filename) == 0) {
            snprintf(out_path, out_size, "%s", path);
            closedir(dir);
            return 1;
        }
    }

    closedir(dir);
    return 0;
}

void http_send_file(HttpRequest *request, int client_fd, char *filepath) {
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