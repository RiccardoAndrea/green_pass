#ifndef HELPER_H
#define HELPER_H
#include <sys/socket.h>

// Definizione del prototipo della funzione Socket
int Socket(int family, int type, int protocol);

// Definizione del prototipo della funzione Connect
int Connect(int sockFd, const struct sockaddr *addr, socklen_t addrLen);

// Definizione del prototipo della funzione Bind
int Bind(int sockFd, const struct sockaddr *serverAddr, socklen_t addrLen);

// Definizione del prototipo della funzione Listen
int Listen(int sockFd, int queLen);

// Definizione del prototipo della funzione Accept
int Accept(int sockFd, struct sockaddr *restrict clientAddr, socklen_t *restrict addrDime);
#endif
