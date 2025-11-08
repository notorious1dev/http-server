#include <stdio.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include "socket_utilities.h"
#include <string.h>
#include <dummy_parser.h>

#define IP "127.0.0.1"
#define PORT 8080
#define BUFFOR_SIZE 128
char buffor[BUFFOR_SIZE];

const char* response =
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html\r\n"
        "Content-Length: 22\r\n"
        "\r\n"
        "<h1>Hello Browser!</h1>";

int main()
{
    struct sockaddr_in *server_socket = sockaddr_server_constructor(PORT);
    int* server_fd = initialize_server(server_socket);

    printf("Server: Listening port: %d\n", PORT);

    while(1)
    {
        struct sockaddr_in client;
        socklen_t lenght = sizeof(client);
        int client_fd = accept(*server_fd, (struct sockaddr*)&client, &lenght);
        if(client_fd < 0) {
            close(*server_fd);
            printf("Server: error with handling connection.\n");
            break;
        }

        printf("Server: new connection from: %s:%d\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));

        ssize_t read_bytes = read(client_fd, buffor, BUFFOR_SIZE);

        HttpRequest parsed_http = http_parse_request(buffor, BUFFOR_SIZE);

        size_t send_bytes = send(client_fd, response, strlen(response), 0);
        if (send_bytes == strlen(response)) {
            printf("Server: data delivered successfully\n");
        }
        
        printf("Server: closing the connection %s:%d\n", inet_ntoa(client.sin_addr), htons(client.sin_port));
        free(parsed_http.method);
        free(parsed_http.path);
        close(client_fd);

    }
}
