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
#define REQUEST_FILE_BUFFER 1024
#define PAGES_DIR "./pages"

typedef struct request_t {
    struct sockaddr_in client;
    int fd_client;
    char *msg;
} request_t;

void handle_file_send(HttpRequest *request, int client_fd);
void* process_request(void* rq);

int main()
{
    struct sockaddr_in *server_socket = sockaddr_server_constructor(PORT);
    int server_fd = initialize_server(server_socket);
    threadpool_t * threadpool = threadpool_init(THREADS_AMOUNT);
    printf("Server: Listening on port: %d\n", PORT);

    while (1)
    {
        struct sockaddr_in client;
        socklen_t lenght = sizeof(client);
        int client_fd = accept(server_fd, (struct sockaddr *)&client, &lenght);

        if (client_fd < 0)
        {
            perror("Server: accept error");
            continue; 
        }
        printf("Server: new connection from: %s:%d\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));

        request_t * request = malloc(sizeof(request_t));
        request->client = client;
        request->fd_client = client_fd;

        threadpool_create_work(threadpool, request, process_request);
    }

    close(server_fd);
    return 0;
}

void* process_request(void* rq)
{
    request_t* request = (request_t*)rq;

    request->msg = (char*)malloc(sizeof(char) * BUFFER_SIZE);
    if (!request->msg) {
        close(request->fd_client);
        free(request);
        return NULL;
    }

    ssize_t bytes_read = read(request->fd_client, request->msg, BUFFER_SIZE - 1);
    if (bytes_read <= 0)
    {
       printf("Server: read error or empty request.\n");
       close(request->fd_client);
       free(request->msg);
       free(request);
       return NULL;
    }
    request->msg[bytes_read] = '\0';

    HttpRequest *parsed_http = http_parse_request(request->msg, BUFFER_SIZE);        
    if (parsed_http == NULL) {
        printf("ERROR: http request was not parsed\n");
        close(request->fd_client);
        free(parsed_http);

        free(request->msg);
        free(request);
        return NULL;
    } 

    printf("PATH:'%s' + METHOD:%s\n", parsed_http->path, parsed_http->method);
    handle_file_send(parsed_http, request->fd_client);

    free(parsed_http->method);
    free(parsed_http->path);
    free(parsed_http);

cleanup_wrapper:
    printf("Server: closing connection %s:%d\n", inet_ntoa(request->client.sin_addr), ntohs(request->client.sin_port));
    close(request->fd_client);
    free(request->msg);
    free(request);
    
    return NULL;
}

//----------FUNCTIONS IMPLEMENTATION-----------//
void handle_file_send(HttpRequest *request, int client_fd)
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