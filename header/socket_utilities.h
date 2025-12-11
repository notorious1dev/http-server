#ifndef SOCKET_UTILITIES
#define SOCKET_UTILITIES
#include <sys/socket.h>

void sockaddr_server_constructor(struct sockaddr_in*, int);
int initialize_server(struct sockaddr_in*);

typedef enum SOCKET_STATUS
{
    SOCKET_NOT_CREATED,
    SOCKET_CREATED,
}SOCKET_STATUS;

#endif