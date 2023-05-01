#include "server.h"

int createServer() {
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
  return server_socket;
}

void runServer(int server_socket) {
  int client_socket;
  while (true) {
    client_socket = accept(server_socket, NULL, NULL);
    if (client_socket == -1) {
      perror("accept");
      continue;
    }
    if (!fork()) {
      close(server_socket);
    }
    // handler failed
    close(client_socket);
  }
}
