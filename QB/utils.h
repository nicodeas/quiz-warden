#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <unistd.h>
extern void compileC(char *fileName, char *outputFile);
extern char *runC(char *execName);
extern char *runPython(char *execName);
extern void sendFile(char *fname, int client_socket);
