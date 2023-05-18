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
    printf("question id: %i\n", questionId);
    request->user_answer = strdup(strtok(NULL, REQUEST_DELIM));
    printf("user answer: %s\n", request->user_answer);
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
  // construct response
  char response[size + 1];
  sprintf(response, "%i&%s&%s&%s", request->question->id, language, type, text);
  if (request->question->type == CHOICE) {
    sprintf(response + strlen(response), "&%s^%s^%s^%s",
            request->question->choices->a, request->question->choices->b,
            request->question->choices->c, request->question->choices->d);
  }
  // send response
  send(request->client_socket, response, strlen(response), 0);
}

void markQuestion(Request *request) {
  if (DEBUG) {
    printf("=====\tMarking Question Summary\t=====\n");
    printf("id:\t\t %d\n", request->question->id);
    printf("Question:\t%s\n", request->question->text);
    printf("Language: \t %s\n",
           request->question->language == PYTHON ? "PYTHON" : "CLANG");
    printf("User's answer:\n ");
    printf("%s\n", request->user_answer);
    printf("=====\tEnd of Summary\t=====\n");
  }
  char response[BUFSIZ];
  memset(response, 0, sizeof(response));
  // choice and image questions have same marking procedure
  if (request->question->type == CHOICE) {
    if (strcmp(request->question->answer, request->user_answer) == 0) {
      send(request->client_socket, "correct", strlen("correct"), 0);
      return;
    } else {
      send(request->client_socket, "incorrect", strlen("incorrect"), 0);
      return;
    }
  } else if (request->question->type == IMAGE) {
    FILE *answerFile;

    answerFile = fopen(PYTHON_USER_ANSWER_PATH, "w");
    fprintf(answerFile, request->user_answer, strlen(request->user_answer));
    fclose(answerFile);

    int answerFd = runCode(request);
    if (answerFd == -1) {
      send(request->client_socket, "Execution timed out",
           strlen("Execution timed out"), 0);
      return;
    }

    FILE *user_output = fopen(request->question->imageFile, "rb");
    FILE *expected_output = fopen(request->question->answerFile, "rb");

    int c1, c2;
    do {
        c1 = fgetc(user_output);
        c2 = fgetc(expected_output);
        if (c1 != c2) {
          send(request->client_socket, "incorrect", strlen("incorrect"), 0);
          return;
        }
    } while (c1 != EOF && c2 != EOF);

    send(request->client_socket, "correct", strlen("correct"), 0);
    return;
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
      int result = compileC();
      if (result != -1) {
        FILE *errorFile = fdopen(result, "r");
        char error[4096];
        memset(error, 0, sizeof(error));
        fgets(error, 4096, errorFile);
        fclose(errorFile);
        close(result);
        sprintf(response, "ERROR|%s", error);
        send(request->client_socket, response, strlen(response), 0);
        return;
      }
      break;
    }

    int answerFd = runCode(request);
    if (answerFd == -1) {
      send(request->client_socket, "Execution timed out",
           strlen("Execution timed out"), 0);
      return;
    }

    FILE *executionOutputFile = fdopen(answerFd, "r");
    char answer[4096];
    memset(answer, 0, sizeof(answer));
    fgets(answer, 4096, executionOutputFile);
    fclose(executionOutputFile);
    close(answerFd);
    // Compare answers
    if (strcmp(request->question->answer, answer) == 0) {
      sprintf(response, "CORRECT|");
      send(request->client_socket, response, strlen(response), 0);
    } else {
      sprintf(response, "INCORRECT|%s", answer);
      send(request->client_socket, response, strlen(response), 0);
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
