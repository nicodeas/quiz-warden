#include "server.h"


// helper functions for GENERATE_QUESTIONS
// here for debugging; move these to other file
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

void parseRequest(Request *request) {
  // will not handle requests that are larger than bufsiz for this
  // project, question/answers should not be that long

  // TODO: might want what happens when a non existent question gets requested
  char requestBuffer[BUFSIZ];
  recv(request->client_socket, requestBuffer, sizeof(requestBuffer), 0);

  char *token = strtok(requestBuffer, REQUEST_DELIM);
  // NOTE: don't mind the if else statements
  // can't exactly run a switch case on strings :(
  if (strcmp(token, "GENERATE_QUESTIONS") == 0) {
    request->action = GENERATE_QUESTIONS;
    request->session_token = atoi(strtok(NULL, REQUEST_DELIM));
  } else if (strcmp(token, "MARK_QUESTION_BY_ID") == 0) {
    token = strtok(NULL, REQUEST_DELIM);
    int questionId = atoi(token);

    request->question = QUESTION_BANK[questionId];
    request->attempt = strdup(strtok(NULL, REQUEST_DELIM));
    request->action = MARK_QUESTION_BY_ID;
  } else if (strcmp(token, "GET_QUESTION_BY_ID") == 0) {
    token = strtok(NULL, REQUEST_DELIM);
    int questionId = atoi(token);

    request->question = QUESTION_BANK[questionId];
    request->action = GET_QUESTION_BY_ID;
  } else if (strcmp(token, "HEALTH_CHECK") == 0) {
    request->action = HEALTH_CHECK;
  }
}

void handleRequest(int client_socket) {
  // TODO: deal with multiple coding/image questions needing to be marked, need
  // some way of locking the exe file
  printf("handling request ...\n");
  Request *request = newRequest(client_socket);
  parseRequest(request);

  // debug
  if (request->question) {
    printf("Request question %s\n", request->question->text);
  }
  if (request->attempt) {
    printf("Request Question attempt: %s\n", request->attempt);
  }

  // handle based on action
  switch (request->action) {
  case (GENERATE_QUESTIONS):;
    int *questions = generateRandomQuestionIds(NUM_QUESTIONS, request->session_token);
    
    for (int i = 0; i < NUM_QUESTIONS; i++) {
      char id_str[12];
      sprintf(id_str,"%i&", questions[i]);
      send(request->client_socket, id_str, strlen(id_str), 0);
    }
    break;
  case (MARK_QUESTION_BY_ID):;
    break;
  case (GET_QUESTION_BY_ID):;
    // '&' delims elements of question, '^' delims multi-choice, '$' delims file
    char id_str[12];
    sprintf(id_str, "%i", request->question->id);
    send(request->client_socket, id_str, strlen(id_str), 0);
    send(request->client_socket, "&", 1, 0);
    send(request->client_socket, QuestionLanguageToString(request->question->language), strlen(QuestionLanguageToString(request->question->language)), 0);
    send(request->client_socket, "&", 1, 0);
    send(request->client_socket, QuestionTypeToString(request->question->type), strlen(QuestionTypeToString(request->question->type)), 0);
    send(request->client_socket, "&", 1, 0);
    send(request->client_socket, request->question->text, strlen(request->question->text), 0);

    if (request->question->type == CHOICE) {
        send(request->client_socket, "&", 1, 0);
        send(request->client_socket, request->question->choices->a, strlen(request->question->choices->a), 0);
        send(request->client_socket, "^", 1, 0);
        send(request->client_socket, request->question->choices->b, strlen(request->question->choices->b), 0);
        send(request->client_socket, "^", 1, 0);
        send(request->client_socket, request->question->choices->c, strlen(request->question->choices->c), 0);
        send(request->client_socket, "^", 1, 0);
        send(request->client_socket, request->question->choices->d, strlen(request->question->choices->d), 0);
    }
    if (request->question->type == IMAGE) {
        send(request->client_socket, "&", 1, 0);
        send(request->client_socket, request->question->imageFile,  strlen(request->question->imageFile), 0);
        send(request->client_socket, "$", 1, 0);
        sendFile(request->question->imageFile,request->client_socket);
    }
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