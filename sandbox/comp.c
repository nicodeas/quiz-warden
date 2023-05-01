#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
  char *fname = "hello.c";
  int pid;
  pid = fork();
  if (pid == -1) {
    perror("fork");
    exit(EXIT_FAILURE);
  } else if (pid == 0) {
    // compile code here
    execl("/bin/cc", "cc", "-o", "tmp", fname, NULL);
    printf("Error during compilation");
    exit(EXIT_FAILURE);
  }
  // wait for compilation to complete, program will fail without this
  wait(NULL);

  int fd[2];
  if (pipe(fd) == -1) {
    perror("pipe");
    exit(EXIT_FAILURE);
  }

  pid = fork();
  if (pid == -1) {
    perror("fork");
    exit(EXIT_FAILURE);
  } else if (pid == 0) {
    dup2(fd[1], STDOUT_FILENO);
    close(fd[0]);
    close(fd[1]);
    /* execl("/bin/python3", "python3", "main.py", NULL); */
    execl("./tmp", "./tmp", NULL);
    printf("failed to run code");
    exit(EXIT_FAILURE);
  }
  close(fd[1]);
  FILE *stream;
  stream = fdopen(fd[0], "r");
  char line[BUFSIZ];
  fgets(line, BUFSIZ, stream);
  if (strcmp(line, "25\n") == 0) {
    printf("answer is correct!\n");
  } else {
    printf("answer incorrect!\n");
  }
  return 0;
}
