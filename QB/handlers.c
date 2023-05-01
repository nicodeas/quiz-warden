#include "handlers.h"

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
  /* sendFile("test.txt", client_socket); */

  close(client_socket);
  exit(EXIT_SUCCESS);
}
