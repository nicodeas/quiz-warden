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

void compileC() {
  int pid;
  pid = fork();
  if (pid == -1) {
    perror("fork");
    exit(EXIT_FAILURE);
  } else if (pid == 0) {
    // compile code
    execl(PATH_C, "cc", "-o", USER_ANSWER_EXE_PATH, CLANG_USER_ANSWER_PATH,
          NULL);
    fprintf(stderr, "Error during compilation\n");
    exit(EXIT_FAILURE);
  }
  // wait for compilation to complete
  // TODO: change to waitpid to check for error code etc
  wait(NULL);
}

int runC() {
  // NOT IN USE
  int fd[2];
  if (pipe(fd) == -1) {
    perror("pipe");
    exit(EXIT_FAILURE);
  }
  int pid;
  pid = fork();
  if (pid == -1) {
    perror("fork");
    exit(EXIT_FAILURE);
  } else if (pid == 0) {
    dup2(fd[0], STDIN_FILENO);
    dup2(fd[1], STDOUT_FILENO);
    close(fd[1]);
    close(fd[0]);
    execl(USER_ANSWER_EXE_PATH, USER_ANSWER_EXE_PATH, NULL);
  }
  close(fd[1]);
  // return read end for checking answer
  return fd[0];
}

int runCode(Request *request) {
  if (DEBUG) {
    printf("Executing User's code\n");
  }
  int fd[2];
  if (pipe(fd) == -1) {
    perror("pipe");
    exit(EXIT_FAILURE);
  }
  int pid;
  pid = fork();
  if (pid == -1) {
    perror("fork");
    exit(EXIT_FAILURE);
  } else if (pid == 0) {
    dup2(fd[1], STDOUT_FILENO);
    close(fd[0]);
    close(fd[1]);
    switch (request->question->language) {
    case (PYTHON):
      execl(PATH_PYTHON, "python3", PYTHON_USER_ANSWER_PATH, NULL);
      fprintf(stderr, "failed to run code");
      exit(EXIT_FAILURE);
      break;
    case (CLANG):
      execl(USER_ANSWER_EXE_PATH, USER_ANSWER_EXE_PATH, NULL);
      fprintf(stderr, "failed to run code");
      exit(EXIT_FAILURE);
      break;
    default:
      fprintf(stderr, "Language not found!\n");
      exit(EXIT_FAILURE);
    }
  }
  close(fd[1]);
  return fd[0];
}

Request *newRequest(int client_socket) {
  Request *request = (Request *)malloc(sizeof(Request));
  request->user_answer = NULL;
  request->client_socket = client_socket;
  request->action = UNSPEC;
  request->question = NULL;
  return request;
}

void freeRequest(Request *request) {
  free(request->user_answer);
  free(request);
}

int *generateRandomQuestionIds(int numQuestions) {
  int *questions = malloc(numQuestions * sizeof(int));
  // hash set for added questions
  bool added[NUM_QUESTIONS];
  // set all to false
  memset(added, 0, NUM_QUESTIONS);
  // time seeded rng
  srand(time(NULL));
  int count = 0;

  while (count < numQuestions) {
    // generate q_id from NUM_QUESTIONS in QB
    int q_id = rand() % NUM_QUESTIONS;
    // add q_id to questions if not already in set
    if (!added[q_id]) {
      questions[count++] = q_id;
      added[q_id] = true;
    }
  }

  return questions;
}

// helper functions for getQuestions
const char *QuestionTypeToString(QuestionType type) {
  switch (type) {
  case CHOICE:
    return "CHOICE";
  case IMAGE:
    return "IMAGE";
  case CODE:
    return "CODE";
  default:
    return "UNKNOWN";
  }
}

const char *QuestionLanguageToString(QuestionLanguage language) {
  switch (language) {
  case PYTHON:
    return "PYTHON";
  case CLANG:
    return "CLANG";
  default:
    return "UNKNOWN";
  }
}
