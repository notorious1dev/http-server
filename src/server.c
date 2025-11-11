#include <stdio.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include "socket_utilities.h"
#include <string.h>
#include <dummy_parser.h>
#include <sys/sendfile.h>
#include <fcntl.h>
#include <file_utilities.h>

#define IP "127.0.0.1"
#define PORT 8080
#define BUFFOR_SIZE 1024
char buffer[BUFFOR_SIZE];

//-------------FUNCTIONS DECLARATION---------------//
void handle_request(HttpRequest request, int client_fd);

int main()
{
    //Initialization
    struct sockaddr_in *server_socket = sockaddr_server_constructor(PORT);
    int server_fd = initialize_server(server_socket);

    printf("Server: Listening on port: %d\n", PORT);

    while (1)
    {
        //Listening
        struct sockaddr_in client;
        socklen_t lenght = sizeof(client);
        int client_fd = accept(server_fd, (struct sockaddr *)&client, &lenght);

        if (client_fd < 0)
        {
            close(server_fd);
            printf("Server: error with handling connection.\n");
            break;
        }

        printf("Server: new connection from: %s:%d\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));

        //Reading
        ssize_t bytes_read = read(client_fd, buffer, BUFFOR_SIZE);
        if (bytes_read <= 0)
        {
            printf("Server: read error or empty request.\n");
            close(client_fd);
            continue;
        }

        //Handling
        HttpRequest parsed_http = http_parse_request(buffer);
        printf("PATH:%s + METHOD:%s\n", parsed_http.path, parsed_http.method);
        handle_request(parsed_http, client_fd);

        //Closing
        free(parsed_http.method);
        free(parsed_http.path);
        //memset(buffor, 0, BUFFOR_SIZE);
        printf("Server: closing the connection %s:%d\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));
        close(client_fd);
    }

    close(server_fd);
    return 0;
}

//----------FUNCTIONS IMPLEMENTATION-----------//
//Currently it always responses with 200 OK Header
void handle_request(HttpRequest request, int client_fd)
{
    if (strcmp("/", request.path) == 0)
        http_send_file(request, client_fd, "./pages/index.html");
    else
        http_send_file(request, client_fd, "./pages/404.html");
}
