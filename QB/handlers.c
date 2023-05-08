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

// get strlen for each question's elements + delims
size_t str_len_of_questions(int NUM_QUESTIONS, int *questions) {
  size_t total = 0;
  
  for (int i = 0; i < NUM_QUESTIONS; i++) {
    total += snprintf(NULL, 0, "%i", QUESTION_BANK[questions[i]]->id) +1;
    total += strlen(QuestionLanguageToString(QUESTION_BANK[questions[i]]->language)) +1;
    total += strlen(QuestionTypeToString(QUESTION_BANK[questions[i]]->type)) +1;
    total += strlen(QUESTION_BANK[questions[i]]->text);
    if (QUESTION_BANK[questions[i]]->type == CHOICE) {
      total += strlen(QUESTION_BANK[questions[i]]->choices->a) +2;
      total += strlen(QUESTION_BANK[questions[i]]->choices->b) +1;
      total += strlen(QUESTION_BANK[questions[i]]->choices->c) +1;
      total += strlen(QUESTION_BANK[questions[i]]->choices->d);
    }
    if (QUESTION_BANK[questions[i]]->type == IMAGE) {
      total += strlen(QUESTION_BANK[questions[i]]->imageFile) +1;
    }
    total += 1;
  }
  return total;
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
  {
    // TODO: Replace NUM_QUESTIONS with how many questions of type we want in quiz
    // maybe part of the config of each QB? (declare lang and how many questions we want)
    // image file needs to be sent (python end reads data to file?), zip image for easier transfer?
    int *questions = generateRandomQuestionIds(NUM_QUESTIONS, request->session_token);
    char *str = malloc(str_len_of_questions(NUM_QUESTIONS, questions) * sizeof(char));

    int file_queue[NUM_QUESTIONS];
    int file_count = 0;

    // '\n' delims questions, '&' delims elements of question, '^' delims multi-choice
    for (int i = 0; i < NUM_QUESTIONS; i++) {
        char id_str[12];
        sprintf(id_str, "%i", QUESTION_BANK[questions[i]]->id);
        strcat(str,id_str);
        strcat(str, "&");
        strcat(str, QuestionLanguageToString(QUESTION_BANK[questions[i]]->language));
        strcat(str, "&");
        strcat(str, QuestionTypeToString(QUESTION_BANK[questions[i]]->type));
        strcat(str, "&");
        strcat(str, QUESTION_BANK[questions[i]]->text);

        if (QUESTION_BANK[questions[i]]->type == CHOICE) {
            strcat(str, "&");
            strcat(str, QUESTION_BANK[questions[i]]->choices->a);
            strcat(str, "^");
            strcat(str, QUESTION_BANK[questions[i]]->choices->b);
            strcat(str, "^");
            strcat(str, QUESTION_BANK[questions[i]]->choices->c);
            strcat(str, "^");
            strcat(str, QUESTION_BANK[questions[i]]->choices->d);
        }
        if (QUESTION_BANK[questions[i]]->type == IMAGE) {
            strcat(str, "&");
            strcat(str, QUESTION_BANK[questions[i]]->imageFile);
            file_queue[file_count++] = i;
            sendFile(QUESTION_BANK[questions[i]]->imageFile,request->client_socket);
        }
        strcat(str,"\n");
    }
    send(request->client_socket, str, strlen(str), 0);
    break;
  }
  case (MARK_QUESTION_BY_ID):;
    break;
  case (GET_QUESTION_BY_ID):;
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


/*   int *questions = generateRandomQuestionIds(NUM_QUESTIONS, request->session_token);
    char str[4096] = "";
    char token_str[128];

    strcat(str,token_str);
    for (int i = 0; i < NUM_QUESTIONS; i++) {
        char id_str[12];
        sprintf(id_str, "%i", QUESTION_BANK[questions[i]]->id);
        strcat(str, "&");
        strcat(str,id_str);
        strcat(str, QuestionLanguageToString(QUESTION_BANK[questions[i]]->language));
        strcat(str, "&");
        strcat(str, QuestionTypeToString(QUESTION_BANK[questions[i]]->type));
        strcat(str, "&");
        strcat(str, QUESTION_BANK[questions[i]]->text);

        if (QUESTION_BANK[questions[i]]->type == CHOICE) {
            strcat(str, "&");
            strcat(str, QUESTION_BANK[questions[i]]->choices->a);
            strcat(str, "^");
            strcat(str, QUESTION_BANK[questions[i]]->choices->b);
            strcat(str, "^");
            strcat(str, QUESTION_BANK[questions[i]]->choices->c);
            strcat(str, "^");
            strcat(str, QUESTION_BANK[questions[i]]->choices->d);
        }
        strcat(str,"\n");
    }
    send(request->client_socket, str, strlen(str), 0);
    break;*/




  /*case (GENERATE_QUESTIONS):;
    // TODO: Replace NUM_QUESTIONS with how many questions of type we want in quiz
    // malloc using size of question structs?
    // zip image for easier transfer?
    // define delims for question and element
    // image file needs to be sent (python end reads data to file?)
    
    int *questions = generateRandomQuestionIds(NUM_QUESTIONS, request->session_token);
    size_t str_size = 1024;
    size_t str_len = 0;
    char *str = malloc(str_size);
    printf("%lu\n", size_of_questions(NUM_QUESTIONS, questions));

    for (int i = 0; i < NUM_QUESTIONS; i++) {
        char id_str[12];
        sprintf(id_str, "%i", QUESTION_BANK[questions[i]]->id);
        append_str(&str, &str_size, &str_len, id_str);
        append_str(&str, &str_size, &str_len, "&");
        append_str(&str, &str_size, &str_len, QuestionLanguageToString(QUESTION_BANK[questions[i]]->language));
        append_str(&str, &str_size, &str_len, "&");
        append_str(&str, &str_size, &str_len, QuestionTypeToString(QUESTION_BANK[questions[i]]->type));
        append_str(&str, &str_size, &str_len, "&");
        append_str(&str, &str_size, &str_len,QUESTION_BANK[questions[i]]->text);

        if (QUESTION_BANK[questions[i]]->type == CHOICE) {
            append_str(&str, &str_size, &str_len, "&");
            append_str(&str, &str_size, &str_len, QUESTION_BANK[questions[i]]->choices->a);
            append_str(&str, &str_size, &str_len, "^");
            append_str(&str, &str_size, &str_len,QUESTION_BANK[questions[i]]->choices->b);
            append_str(&str, &str_size, &str_len, "^");
            append_str(&str, &str_size, &str_len,QUESTION_BANK[questions[i]]->choices->c);
            append_str(&str, &str_size, &str_len, "^");
            append_str(&str, &str_size, &str_len,QUESTION_BANK[questions[i]]->choices->d);
        }

        if (QUESTION_BANK[questions[i]]->type == IMAGE) {
            append_str(&str, &str_size, &str_len, "&");
            append_str(&str, &str_size, &str_len, QUESTION_BANK[questions[i]]->imageFile);
        }
        append_str(&str, &str_size, &str_len, "\n");
    }

    send(request->client_socket, str, strlen(str), 0);
    break;*/