#include "server.h"

Question *QUESTION_BANK[QUESTION_BANK_SIZE];
int NUM_QUESTIONS = 0;

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
      handleRequest(client_socket);
    }
    // Do not wait if you want to handle requests concurrently
    wait(NULL);
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
  char *text;

  while (fgets(buffer, sizeof(buffer), file) != NULL) {
    if (buffer[0] == '-') {
      // if end of a question, push it to the question bank
      currentQuestion->id = NUM_QUESTIONS;
      QUESTION_BANK[NUM_QUESTIONS++] = currentQuestion;
      continue;
    }

    // remove new line
    buffer[strlen(buffer) - 1] = '\0';
    // allocate space for line start at third character as we have a white space
    // between types
    text = strdup(&buffer[2]);

    switch (buffer[0]) {
    // Question text
    case ('Q'):
      currentQuestion = (Question *)malloc(sizeof(Question));
      currentQuestion->choices = NULL;
      currentQuestion->answer = NULL;
      currentQuestion->imageFile = NULL;
      currentQuestion->answerFile = NULL;
      currentQuestion->text = text;
      currentQuestion->id = -1;
      break;

    // Question type
    case ('T'):
      if (strcmp(text, "CHOICE") == 0) {
        currentQuestion->type = CHOICE;
        currentQuestion->choices = (Choices *)malloc(sizeof(Choices));
      } else if (strcmp(text, "IMAGE") == 0) {
        currentQuestion->type = IMAGE;
      } else if (strcmp(text, "CODE") == 0) {
        currentQuestion->type = CODE;
      } else {
        fprintf(stderr, "Unrecognised character in Question file: %s\n",
                buffer);
        exit(EXIT_FAILURE);
      }
      break;

    // multi choice
    case ('A'):
      currentQuestion->choices->a = text;
      break;
    case ('B'):
      currentQuestion->choices->b = text;
      break;
    case ('C'):
      currentQuestion->choices->c = text;
      break;
    case ('D'):
      currentQuestion->choices->d = text;
      break;

    // answer
    case ('$'):
      currentQuestion->answer = text;
      break;

    // image 1
    case ('I'):
      currentQuestion->imageFile = text;
      break;

    // image 2
    case ('F'):
      currentQuestion->answerFile = text;
      break;

    // language
    case ('L'):
      if (strcmp(text, "PYTHON") == 0) {
        currentQuestion->language = PYTHON;
      } else if (strcmp(text, "CLANG") == 0) {
        currentQuestion->language = CLANG;
      } else {
        fprintf(stderr, "Unrecognised character in Question file: %s\n",
                buffer);
        exit(EXIT_FAILURE);
      }
      break;

    default:
      fprintf(stderr, "Unrecognised character in Question file: %s\n", buffer);
      exit(EXIT_FAILURE);
    };
  }
  fclose(file);
}
