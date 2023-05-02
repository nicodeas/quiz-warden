#include "server.h"

void sendFile(char *fname, int client_socket) {
  // think of this function this way, it is not exactly sending a "file" but
  // rather it is reading a file in chunks and sending that to the other end
  printf("Sending file ...\n");
  FILE *file;
  char buffer[BUFSIZ];
  file = fopen(fname, "rb");
  size_t nread, nsend;

  if (file == NULL) {
    perror("fopen");
    exit(EXIT_FAILURE);
  }

  // number of bytes read is size * count, in this case size = 1 and count =
  // BUFSIZ, it will work the other way round as well
  while ((nread = fread(buffer, 1, BUFSIZ, file)) > 0) {
    nsend = send(client_socket, buffer, nread, 0);
    if (nsend == -1) {
      perror("Send file");
      exit(EXIT_FAILURE);
    }
  }
  fclose(file);
}

void compileC(char *fileName, char *outputFile) {
  int pid;
  pid = fork();
  if (pid == -1) {
    perror("fork");
    exit(EXIT_FAILURE);
  } else if (pid == 0) {
    // compile code
    execl("/bin/cc", "cc", "-o", outputFile, fileName, NULL);
    printf("Error during compilation\n");
    exit(EXIT_FAILURE);
  }
  // wait for compilation to complete
  wait(NULL);
}
