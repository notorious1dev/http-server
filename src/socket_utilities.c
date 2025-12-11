#include <arpa/inet.h>
#include <socket_utilities.h>
#include <unistd.h>

#define MAX_CONNECTIONS 10

void sockaddr_server_constructor(struct sockaddr_in* socket, int port) {
    socket->sin_family = AF_INET;
    socket->sin_addr.s_addr = INADDR_ANY;
    socket->sin_port = htons(port);
}

int initialize_server(struct sockaddr_in* addr)
{
    int server_fd = socket(addr->sin_family, SOCK_STREAM, 0);
    if (server_fd < 0) {
        return SOCKET_NOT_CREATED;
    }

    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        close(server_fd);
        return SOCKET_NOT_CREATED;
    }

    if (bind(server_fd, (struct sockaddr*)addr, sizeof(*addr)) < 0) {
        close(server_fd);
        return SOCKET_NOT_CREATED;
    }

    if (listen(server_fd, MAX_CONNECTIONS) < 0) {
        close(server_fd);
        return SOCKET_NOT_CREATED;
    }

    return server_fd;
}
