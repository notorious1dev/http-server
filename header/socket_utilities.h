#ifndef SOCKET_UTILITIES
#define SOCKET_UTILITIES
#include <stdio.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>

void* sockaddrIP4_constructor(char* ip, int port);

struct sockaddr_in* sockaddr_server_constructor(int port);

int* initialize_server(struct sockaddr_in* socket_data);

#endif