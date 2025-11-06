#include <stdio.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>

#define MAX_CONNECTIONS 10

void* sockaddrIP4_constructor(char* ip, int port) {
    struct sockaddr_in socket = {0};
    socket.sin_family = AF_INET; //ipv4
    socket.sin_port = htons(port);
    
    if (inet_pton(AF_INET, ip, &(socket.sin_addr)) != 1) {
        printf("Error: adress convertation\n");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in* socket_return = malloc(sizeof(struct sockaddr_in));
    *socket_return = socket;

    printf("Socket data: %d, %s", port, ip);

    return socket_return;
}

struct sockaddr_in* sockaddr_server_constructor(int port) {
    struct sockaddr_in socket_data = {0};
    socket_data.sin_family = AF_INET;
    socket_data.sin_addr.s_addr = INADDR_ANY;
    socket_data.sin_port = htons(port);

    struct sockaddr_in* return_socket = malloc(sizeof(struct sockaddr_in));
    *return_socket = socket_data;

    return return_socket;;
}

int* initialize_server(struct sockaddr_in* socket_data) {
    int server_fd = socket(socket_data->sin_family, SOCK_STREAM, 0);
    if (server_fd < 0) {
        printf("Error: Server socket was not initialized\n");
        goto clean_up;
    }

    size_t socket_size = sizeof(*socket_data);
    if (bind(server_fd, (struct sockaddr*)socket_data, sizeof(*socket_data)) != 0) {
        printf("Error: server's socket was not binded\n");
        goto clean_up;
    }

    if (listen(server_fd, MAX_CONNECTIONS) < 0) {
        printf("Error: server listening failed\n");
        goto clean_up;
    }

    int* server_fd_return = malloc(sizeof(int));
    *server_fd_return = server_fd;

    printf("Server is listening on port: %d\n", ntohs(socket_data->sin_port));

    return server_fd_return;

    clean_up:
    close(server_fd);
    exit(EXIT_FAILURE);
   
};