#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <assert.h> 

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/sendfile.h>
#include <arpa/inet.h>
#include <fcntl.h>

#include "socket_utilities.h"
#include "file_utilities.h"
#include "http_utilities.h"
#include "threadpool_t.h"

#define THREADS_AMOUNT 10
#define IP "127.0.0.1"
#define PORT 8080
#define BUFFER_SIZE 1024
#define PAGES_DIR "./pages"

typedef struct connection_t {
    struct sockaddr_in client;
    int fd_client;
    char *msg;
} connection_t;

void static_file_send(http_request_t*, int);
void* process_connection(void*);
void close_connection(connection_t*);

int main()
{
    struct sockaddr_in server_socket = {0};
    sockaddr_server_constructor(&server_socket, PORT);
    int server_fd = initialize_server(&server_socket);

    if (server_fd == SOCKET_NOT_CREATED)
    {
        printf("Server: socket was not opened\n");
        return 1;
    }

    threadpool_t * threadpool = threadpool_init(THREADS_AMOUNT);
    printf("Server: Listening on port: %d\n", PORT);

    while (1)
    {
        struct sockaddr_in client = {0};
        socklen_t client_socket_size = sizeof(client);
        int client_fd = accept(server_fd, (struct sockaddr *)&client, &client_socket_size);

        if (client_fd < 0)
        {
            perror("Server: accept error");
            continue; 
        }
        printf("Server: new connection from: %s:%d\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));

        connection_t * request = malloc(sizeof(connection_t));
        request->client = client;
        request->fd_client = client_fd;
        threadpool_create_work(threadpool, request, process_connection);
    }

    close(server_fd);
    return 0;
}

void* process_connection(void* _connection)
{
    connection_t* connection = (connection_t*)_connection;
    connection->msg = (char*)malloc(BUFFER_SIZE);
    if (!connection->msg) {
        close(connection->fd_client);
        free(connection);
        return NULL;
    }

    ssize_t bytes_read = read(connection->fd_client, connection->msg, BUFFER_SIZE - 1);
    if (bytes_read <= 0)
    {
       printf("Server: read error or empty request.\n");
       close_connection(connection);
       return NULL;
    }
    connection->msg[bytes_read] = '\0';

    http_request_t parsed_http = {0};      
    if (http_parse_request(&parsed_http, connection->msg, bytes_read) != PARSE_SUCCESS) {
        printf("ERROR: http request was not parsed\n");
        close_connection(connection);
        return NULL;
    } 

    printf("PATH:'%s' + METHOD:%s\n", parsed_http.path, parsed_http.method);
    static_file_send(&parsed_http, connection->fd_client);

    free(parsed_http.method);
    free(parsed_http.path);

    printf("Server: closing connection %s:%d\n", inet_ntoa(connection->client.sin_addr), ntohs(connection->client.sin_port));
    close_connection(connection);
    return NULL;
}

void static_file_send(http_request_t *request, int client_fd)
{
    if (strcmp("GET", request->method) != 0) {
        request->status = NOT_IMPLEMENTED;
        http_send_file(request, client_fd, "./pages/501.html");
        return;
    }

    if (strcmp("/", request->path) == 0)
    {
        request->status = OK;
        http_send_file(request, client_fd, "./pages/index.html");
        return;
    }

    size_t size_buffer = strlen(PAGES_DIR) + strlen(request->path) + 1;
    char* file_path = (char*)malloc(size_buffer);

    if (file_path == NULL) {
        request->status = NOT_FOUND;
        http_send_file(request, client_fd, "./pages/404.html");
        return;
    }
    snprintf(file_path, size_buffer, "%s%s", PAGES_DIR, request->path);
    
    //If can't access the file
    if (access(file_path, F_OK | R_OK))
    {
        request->status = NOT_FOUND;
        http_send_file(request, client_fd, "./pages/404.html");
        free(file_path);
        return;
    }

    request->status = OK;
    http_send_file(request, client_fd, file_path);
    free(file_path);
}

void close_connection(connection_t* connection) {
    close (connection->fd_client);
    free(connection->msg);
    free(connection);
}