#include "server.h"

int main() {
  int server_socket = createServer();
  runServer(server_socket);
}
