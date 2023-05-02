#include "server.h"

Question *QUESTION_BANK[QUESTION_BANK_SIZE];

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
  return server_socket;
}

void runServer(int server_socket) {
  int client_socket;
  printf("Server is listening for connections on port: %d\n", PORT);
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

void buildQuestionBank() {
  FILE *file;
  char buffer[BUFSIZ];
  file = fopen(QUESTION_FILE, "r");

  if (file == NULL) {
    perror("fopen");
    exit(EXIT_FAILURE);
  }

  Question *currentQuestion = NULL;
  while (fgets(buffer, sizeof(buffer), file) != NULL) {
    switch (buffer[0]) {
    // Question text
    case ('Q'):
      currentQuestion = (Question *)malloc(sizeof(Question));
      currentQuestion->choices = NULL;
      currentQuestion->answer = NULL;
      currentQuestion->testCase = NULL;
      // set id, text, question type, question language, how do we want to deal
      // with image questions? put the filepath?
      break;
      // multi choice
    case ('A'):;
      break;
    case ('B'):;
      break;
    case ('C'):;
      break;
    case ('D'):;
      break;
      // answer
    case ('$'):;
      break;
    // image
    case ('I'):;
      break;
    // coding question stuff
    // test input
    case ('T'):;
      break;
    // test expected output
    case ('O'):;
      break;
    // TODO: maybe a symbol to mark the end of question and push to QB? E or
    // DEFAULT?
    case ('E'):
      // push question to question bank
      QUESTION_BANK[currentQuestion->id] = currentQuestion;
      break;
    };
  }
  fclose(file);
}
