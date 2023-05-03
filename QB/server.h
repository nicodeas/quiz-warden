#define _POSIX_C_SOURCE 200809L // enables strdup()
#include <netinet/in.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define PORT 8080
#define BACKLOG 16

#define PATH_PYTHON "/bin/python3"
#define PATH_C "/bin/cc"

#define QUESTION_FILE "questions.txt"

typedef enum { CHOICE, IMAGE, CODE } QuestionType;
typedef enum { PYTHON, CLANG } QuestionLanguage;

typedef enum {
  GENERATE_QUESTIONS,
  GET_IMAGE_QUESTION,
  GET_CODING_QUESTION,
  MARK_IMAGE_QUESTION,
  MARK_CODING_QUESTION,
  HEALTH_CHECK
} RequestAction;

typedef struct {
  char *a;
  char *b;
  char *c;
  char *d;
} Choices;

typedef struct {
  QuestionType type;
  QuestionLanguage language;
  char *text;
  Choices *choices;
  char *answer;
  char *imageFile;
  char *answerFile; // used to store correct answer for image question
} Question;

#define QUESTION_BANK_SIZE 512

extern Question *QUESTION_BANK[QUESTION_BANK_SIZE];
extern int NUM_QUESTIONS; // write to this when building question bank to keep
                          // track of max index we can go; not sure if this will
                          // be required

// server functions in server.c
extern void buildQuestionBank(); // TODO: setup function on server startup to
                                 // retrieve questions
extern int createServer();
extern void runServer(int server_socket);

// util functions in util.c
extern void compileC(char *fileName, char *outputFile);
extern int runCode(char *exec, QuestionLanguage language);
extern void sendFile(char *fname, int client_socket);

// handler functions in handlers.c
extern void handleRequest(int client_socket);
extern void getQuestion(int client_socket, int questionId);
extern void markQuestion(char *answer); // TODO: answer contains req from client
                                        // parse to get question id
extern void markChoice(int client_socket, int questionId);
extern void markCode(int client_socket, int questionId, int fd); // TODO:
extern void
markImage(int client_socket,
          int questionId); // TODO: send processed image back to TM. Then send
                           // whether answer is correct/wrong?
