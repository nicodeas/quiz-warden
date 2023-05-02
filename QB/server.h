#include <netinet/in.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define PORT 8080
#define BACKLOG 16

typedef enum { CHOICE, IMAGE, CODE } QuestionType;
typedef enum { PYTHON, C } QuestionLanguage;

typedef struct {
  char *a;
  char *b;
  char *c;
  char *d;
} Choices;

typedef struct {
  char *input;
  char *expectedOutput;
} TestCase;

typedef struct {
  QuestionType type;
  QuestionLanguage language;
  int id;
  char *text;
  Choices choices;
  char *answer;
  TestCase testCase; // TODO: coding question, multiple test cases?
} Question;

#define QUESTION_BANK_SIZE 512

extern Question QUESTION_BANK[QUESTION_BANK_SIZE];
extern int NUM_QUESTIONS; // write to this when building question bank to keep
                          // track of max index we can go; not sure if this will
                          // be required

// server functions in server.c
extern void setup(); // possibly to do with whether it is a C or Python QB?
extern void
seedQuestions(); // TODO: setup function on server startup to retrieve questions
extern int createServer();
extern void runServer(int server_socket);

// util functions in util.c
extern void compileC(char *fileName, char *outputFile);
extern char *runC(char *execName);      // TODO:
extern char *runPython(char *execName); // TODO:
extern void sendFile(char *fname, int client_socket);

// handler functions in handlers.c
extern void handleRequest(int client_socket);
