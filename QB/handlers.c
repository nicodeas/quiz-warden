#include "server.h"

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
    break;
  case (MARK_QUESTION_BY_ID):;
    break;
  case (GET_QUESTION_BY_ID):;
    break;
  case (HEALTH_CHECK):;
    // debug stuff, remove later
    char *resp = "HEALTH_CHECK OK!";
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
