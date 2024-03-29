#define _POSIX_C_SOURCE 200809L // enables strdup()
#include <getopt.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#define BACKLOG 16

#define EXE_TIMEOUT 2
#define PATH_PYTHON "/usr/bin/python3"
#define PATH_C "/usr/bin/cc"

#define PYTHON_QUESTION_FILE "questions_python.txt"
#define CLANG_QUESTION_FILE "questions_clang.txt"

#define CLANG_USER_ANSWER_PATH "./code.c"
#define PYTHON_USER_ANSWER_PATH "./code.py"

#define USER_ANSWER_EXE_PATH "./compiled_code"
#define USER_OUTPUT_PNG "./static/img/user_output.png"

#define REQUEST_DELIM "|"

typedef enum { CHOICE, IMAGE, CODE } QuestionType;
typedef enum { PYTHON, CLANG } QuestionLanguage;

typedef enum {
  GENERATE_QUESTIONS,
  MARK_QUESTION_BY_ID,
  GET_QUESTION_BY_ID,
  GET_ANSWER_BY_ID,
  HEALTH_CHECK,
  UNSPEC
} RequestAction;

typedef struct {
  char *a;
  char *b;
  char *c;
  char *d;
} Choices;

typedef struct {
  int id; // id is required to get the original map the questions back to index
          // when order is jumbled on the TM
  QuestionType type;
  QuestionLanguage language;
  char *text;
  Choices *choices;
  char *answer;
  char *imageFile;
  char *answerFile;
} Question;

typedef struct {
  int client_socket;
  RequestAction action;
  Question *question;
  int num_to_generate;
  char *user_answer;
} Request;

#define QUESTION_BANK_SIZE 512

extern int PORT;
extern char *LANGUAGE; // the language the server is running in
extern char *QUESTION_FILE;

extern Question *QUESTION_BANK[QUESTION_BANK_SIZE];
extern int NUM_QUESTIONS; // write to this when building question bank to keep
                          // track of max index we can go; not sure if this will
                          // be required
extern bool DEBUG;

extern int timedPid;

// server functions in server.c
extern void buildQuestionBank();
extern int createServer();
extern void runServer(int server_socket);

// util functions in util.c
extern int compileC();
extern int runCode(Request *request);
extern void sendFile(char *fname, Request *request);
extern Request *newRequest(int client_socket);
extern void freeRequest(Request *request);
extern int *generateRandomQuestionIds(int numQuestions);
extern const char *QuestionLanguageToString(QuestionLanguage language);
extern const char *QuestionTypeToString(QuestionType type);
extern void saveAnswerLocally(Request *request);
extern void handleAlarm(int sig);

// handler functions in handlers.c
extern void parseRequest(Request *request);
extern void handleRequest(int client_socket);
extern void getQuestion(Request *request);
extern void markQuestion(Request *request);

// debug functions
extern void printQuestionBank();
