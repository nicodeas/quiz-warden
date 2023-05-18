#include "server.h"

void parseRequest(Request *request) {
  // will not handle requests that are larger than bufsiz for this
  // project, question/answers should not be that long

  // TODO: might want what happens when a non existent question gets requested
  char requestBuffer[BUFSIZ];
  memset(requestBuffer, 0, sizeof(requestBuffer));
  recv(request->client_socket, requestBuffer, sizeof(requestBuffer), 0);

  if (DEBUG) {
    printf("=====\tRequest Buffer received\t=====\n");
    printf("%s\n", requestBuffer);
    printf("=====\tEnd of Request\t=====\n");
  }

  char *token = strtok(requestBuffer, REQUEST_DELIM);
  // NOTE: don't mind the if else statements
  // can't exactly run a switch case on strings :(
  if (strcmp(token, "GENERATE_QUESTIONS") == 0) {
    request->action = GENERATE_QUESTIONS;
    request->num_to_generate = atoi(strtok(NULL, REQUEST_DELIM));
  } else if (strcmp(token, "MARK_QUESTION_BY_ID") == 0) {
    request->action = MARK_QUESTION_BY_ID;
    token = strtok(NULL, REQUEST_DELIM);
    int questionId = atoi(token);
    request->question = QUESTION_BANK[questionId];
    request->user_answer = strdup(strtok(NULL, REQUEST_DELIM));
  } else if (strcmp(token, "GET_QUESTION_BY_ID") == 0) {
    request->action = GET_QUESTION_BY_ID;
    token = strtok(NULL, REQUEST_DELIM);
    int questionId = atoi(token);
    request->question = QUESTION_BANK[questionId];
  } else if (strcmp(token, "HEALTH_CHECK") == 0) {
    request->action = HEALTH_CHECK;
  }
}

void getQuestion(Request *request) {
  const char *language = QuestionLanguageToString(request->question->language);
  const char *type = QuestionTypeToString(request->question->type);
  const char *text = request->question->text;

  // determine size of response
  // '&' delims elements of question, '^' delims multi-choice, '$' delims file
  size_t size = snprintf(NULL, 0, "%i&%s&%s&%s", request->question->id,
                         language, type, text);
  if (request->question->type == CHOICE) {
    size +=
        snprintf(NULL, 0, "&%s^%s^%s^%s", request->question->choices->a,
                 request->question->choices->b, request->question->choices->c,
                 request->question->choices->d);
  }
  if (request->question->type == IMAGE) {
    size += snprintf(NULL, 0, "&%s^%s$", request->question->image1,
                     request->question->image2);
  }
  // construct response
  char response[size + 1];
  sprintf(response, "%i&%s&%s&%s", request->question->id, language, type, text);
  if (request->question->type == CHOICE) {
    sprintf(response + strlen(response), "&%s^%s^%s^%s",
            request->question->choices->a, request->question->choices->b,
            request->question->choices->c, request->question->choices->d);
  }
  if (request->question->type == IMAGE) {
    sprintf(response + strlen(response), "&%s^%s$", request->question->image1,
            request->question->image2);
  }
  // send response
  send(request->client_socket, response, strlen(response), 0);
  // send image if required
  if (request->question->type == IMAGE) {
    // send both files
    sendFile(request->question->image1, request->client_socket);
    sendFile(request->question->image2, request->client_socket);
  }
}
void markCode(Request *request) {
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
    switch (request->question->language) {
    case PYTHON:
      execl(PATH_PYTHON, "python3", NULL);
    case CLANG:
      execl(PATH_C, "cc", "-x", "c", "-", "-o", "tmp", NULL);
    }
  }
  printf("question lang %s\n",
         request->question->language == PYTHON ? "python" : "clang");
  printf("user_answer %s\n", request->user_answer);

  write(fd[1], request->user_answer, 50);
  close(fd[1]);
  return;
}

void markQuestion(Request *request) {
  if (DEBUG) {
    printf("Marking question with id:\t %d\n", request->question->id);
    printf("Question:\t%s\n", request->question->text);
    printf("Question language: \t %s\n",
           request->question->language == PYTHON ? "PYTHON" : "CLANG");
    printf("=====\tUser's attempt\t=====\n ");
    printf("%s\n", request->user_answer);
    printf("=====\tEnd of User's attempt\t=====\n");
  }
  // choice and image questions have same marking procedure
  if (request->question->type == CHOICE || request->question->type == IMAGE) {
    if (strcmp(request->question->answer, request->user_answer) == 0) {
      send(request->client_socket, "correct", strlen("correct"), 0);
    } else {
      send(request->client_socket, "incorrect", strlen("incorrect"), 0);
    }
  } else if (request->question->type == CODE) {
    // save to tmp file so we do not need to deal with piping into interpreter
    FILE *answerFile;

    // a little messy but can tidy up down the track
    switch (request->question->language) {
    case PYTHON:
      answerFile = fopen(PYTHON_USER_ANSWER_PATH, "w");
      fprintf(answerFile, request->user_answer, strlen(request->user_answer));
      fclose(answerFile);
      break;
    case CLANG:
      answerFile = fopen(CLANG_USER_ANSWER_PATH, "w");
      fprintf(answerFile, request->user_answer, strlen(request->user_answer));
      fclose(answerFile);
      compileC();
      break;
    }

    int answerFd = runCode(request);
    FILE *executionOutputFile = fdopen(answerFd, "r");
    char answer[BUFSIZ];
    memset(answer, 0, sizeof(answer));
    fgets(answer, BUFSIZ, executionOutputFile);
    fclose(executionOutputFile);
    close(answerFd);

    // Compare answers
    if (strcmp(request->question->answer, answer) == 0) {
      send(request->client_socket, "correct", strlen("correct"), 0);
    } else {
      send(request->client_socket, "incorrect", strlen("incorrect"), 0);
    }
  }
}

void handleRequest(int client_socket) {
  // TODO: deal with multiple coding/image questions needing to be marked, need
  // some way of locking the exe file
  printf("handling request ...\n");
  Request *request = newRequest(client_socket);
  parseRequest(request);

  // handle based on action
  switch (request->action) {
  case (GENERATE_QUESTIONS):;
    int *questions = generateRandomQuestionIds(request->num_to_generate);

    for (int i = 0; i < request->num_to_generate; i++) {
      char id_str[12];
      sprintf(id_str, "%i&", questions[i]);
      send(request->client_socket, id_str, strlen(id_str), 0);
    }
    break;
  case (MARK_QUESTION_BY_ID):;
    markQuestion(request);
    break;
  case (GET_QUESTION_BY_ID):;
    getQuestion(request);
    break;
  case (HEALTH_CHECK):;
    // debug stuff, remove later
    char *resp = LANGUAGE;
    send(request->client_socket, resp, strlen(resp), 0);
    break;
  case (UNSPEC):;
    break;
  }

  // clean up resources, possibly add a function to remove tmp file created by
  // compilation
  freeRequest(request);
  close(client_socket);
  exit(EXIT_SUCCESS);
}
