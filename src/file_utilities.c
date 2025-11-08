#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <dummy_parser.h>
#include <sys/socket.h>
#include <sys/sendfile.h>

size_t file_size(char *filepath) {
    int file = open(filepath, O_RDONLY);

    if (file == -1) {
        printf("file %s was not found\n", filepath);
        return 0;
    }

    off_t size = lseek(file, 0, SEEK_END);
    lseek(file, 0, SEEK_SET);
    close(file);

    return size;

}

void http_send_file(HttpRequest request, int client_fd, char *filepath) {
        size_t size = file_size(filepath);
        int fd = open(filepath, O_RDONLY);
        off_t offset = 0;

        if (fd == -1) {
            printf("Error opening %s\n", filepath);
            return;
        }

        char header[512];
        int header_len = snprintf(header, sizeof(header),
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/html\r\n"
            "Content-Length: %zu\r\n"
            "\r\n", size);

        if (send(client_fd, header, header_len, 0) < 0) {
            printf("Error: Header send failed");
            close(fd);
            return;
        }

        sendfile(client_fd, fd, &offset, size);
        close(fd);
}