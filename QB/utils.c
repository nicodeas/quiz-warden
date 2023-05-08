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
    execl(PATH_C, "cc", "-o", outputFile, fileName, NULL);
    fprintf(stderr, "Error during compilation\n");
    exit(EXIT_FAILURE);
  }
  // wait for compilation to complete
  wait(NULL);
}

int runCode(char *exec, QuestionLanguage language) {
  // NOTE: not sure if we should use exec name or abs path
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
    char execName[64];

    // create exec string for C code, not in use for python
    char *execStart = "./";
    strcpy(execName, execStart);
    strcat(execName, exec);

    switch (language) {
    case (PYTHON):
      execl(PATH_PYTHON, "python3", exec, NULL);
      fprintf(stderr, "failed to run code");
      exit(EXIT_FAILURE);
      break;
    case (CLANG):
      execl(execName, execName, NULL);
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
  request->attempt = NULL;
  request->client_socket = client_socket;
  request->action = UNSPEC;
  request->question = NULL;
  return request;
}

void freeRequest(Request *request) {
  free(request->attempt);
  free(request);
}

int *generateRandomQuestionIds(int numQuestions, int session_token) {
  int *questions = malloc(numQuestions * sizeof(int));
  // hash set for added questions
  bool added[NUM_QUESTIONS];
  // set all to false
  memset(added, 0, NUM_QUESTIONS);
  // session_token seeds rng
  srand(session_token);
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
