#include <netinet/in.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define PORT 8080
#define BACKLOG 16

// server
extern int createServer();
extern void runServer(int server_socket);

// utils
extern void compileC(char *fileName, char *outputFile);
extern char *runC(char *execName);
extern char *runPython(char *execName);
extern void sendFile(char *fname, int client_socket);

// handlers
extern void handleRequest(int client_socket);
