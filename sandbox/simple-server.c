#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#include <sys/socket.h>
#include <sys/types.h>

#include <netinet/in.h>

#define PORT 8080
#define BACKLOG 10

void sendFile(char *fname, int client_socket) {
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
      perror("sending file");
      exit(EXIT_FAILURE);
    }
  }

  fclose(file);
}

void handleRequest(int client_socket) {
  printf("handling request ...\n");
  // use send and recv and read into these buffers
  char requestBuffer[BUFSIZ];
  char responseBuffer[BUFSIZ];
  // TODO: think way to request/ for different resources
  // question sending

  // TODO: remote code execution and checking function

  // TODO: read in questions from text file; could this involve reading into a
  // data structure?

  // send file function for picture question
  sendFile("test.txt", client_socket);

  close(client_socket);
  exit(EXIT_SUCCESS);
}

int main() {
  int server_socket;
  server_socket = socket(AF_INET, SOCK_STREAM, 0);

  struct sockaddr_in server_address;
  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(PORT);
  server_address.sin_addr.s_addr = INADDR_ANY;

  // bind takes in a sockaddr, cast to correct type
  int bind_success = bind(server_socket, (struct sockaddr *)&server_address,
                          sizeof(server_address));
  if (bind_success == -1) {
    close(server_socket);
    perror("bind");
    exit(EXIT_FAILURE);
  }

  int listen_success = listen(server_socket, BACKLOG);
  if (listen_success == -1) {
    perror("listen");
    exit(EXIT_FAILURE);
  }

  printf("Server is listening for connections on port: %d\n", PORT);

  int client_socket;
  while (1) {
    // you could put in structs in the 2nd and 3rd parameters to fill in
    // network info on where the connection is coming from but in this case, we
    // know it is our local machine hence leave it as null
    client_socket = accept(server_socket, NULL, NULL);
    if (client_socket == -1) {
      perror("accept");
      continue;
    }
    if (!fork()) {
      close(server_socket);
      // NOTE: remember to exit from child process
      handleRequest(client_socket);
    }
    // handle request failed
    close(client_socket);
  }
  exit(EXIT_SUCCESS);
}
