#include "server.h"

int main() {
  buildQuestionBank();
  int server_socket = createServer();
  runServer(server_socket);
}
