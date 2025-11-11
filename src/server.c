#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/sendfile.h>
#include <arpa/inet.h>
#include <fcntl.h>

#include "socket_utilities.h"
#include "file_utilities.h"
#include "http_utilities.h"

#define IP "127.0.0.1"
#define PORT 8080
#define BUFFER_SIZE 1024
#define REQUEST_FILE_BUFFER 1024

#define PAGES_DIR "./pages"

char buffer[BUFFER_SIZE];

//-------------FUNCTIONS DECLARATION---------------//
void handle_request(HttpRequest *request, int client_fd);

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
        ssize_t bytes_read = read(client_fd, buffer, BUFFER_SIZE);
        if (bytes_read <= 0)
        {
            printf("Server: read error or empty request.\n");
            close(client_fd);
            continue;
        }

        //Handling
        HttpRequest *parsed_http = http_parse_request(buffer, BUFFER_SIZE);
        if (parsed_http == NULL) {
            printf("ERROR: http request was not parsed\n");
            goto close;
        }

        printf("PATH:'%s' + METHOD:%s\n", parsed_http->path, parsed_http->method);
        handle_request(parsed_http, client_fd);

        close:
        free(parsed_http->method);
        free(parsed_http->path);
        free(parsed_http);
        printf("Server: closing the connection %s:%d\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));
        close(client_fd);
    }

    close(server_fd);
    return 0;
}

//----------FUNCTIONS IMPLEMENTATION-----------//
void handle_request(HttpRequest *request, int client_fd)
{
    if (strcmp("GET", request->method) != 0) {
        request->status = NOT_IMPLEMENTED;
        http_send_file(request, client_fd, "./pages/501.html");
        return;
    }

    if (strcmp("/", request->path) == 0)
    {
        http_send_file(request, client_fd, "./pages/index.html");
        return;
    }

    char filepath[REQUEST_FILE_BUFFER] = {0};
    int find_file_status = find_file(request, PAGES_DIR, filepath, REQUEST_FILE_BUFFER);

    
    if (find_file_status) 
    {
        request->status = OK;
        http_send_file(request, client_fd, filepath);
        return;
    }

    request->status = NOT_FOUND;
    http_send_file(request, client_fd, "./pages/404.html");
}

