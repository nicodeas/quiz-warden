#include "server.h"

void printQuestionBank() {
  printf("=====\tQuestions\t=====\n");
  for (int i = 0; i < NUM_QUESTIONS; i++) {
    Question *question = QUESTION_BANK[i];
    printf("Question ID: %d\n", question->id);
    printf("Question: %s\n", question->text);
    printf("Question Language: %s\n",
           question->language == PYTHON ? "PYTHON" : "CLANG");
    switch (question->type) {
    case IMAGE:
      printf("Question type: IMAGE\n");
      printf("\tIMAGE file1: %s\n", question->imageFile);
      printf("\tIMAGE file2: %s\n", question->answerFile);
      break;
    case CODE:
      printf("Question type: CODE\n");
      printf("Answer: %s\n", question->answer);
      break;
    case CHOICE:
      printf("Question type: CHOICE\n");
      printf("\t\tA: %s\n", question->choices->a);
      printf("\t\tB: %s\n", question->choices->b);
      printf("\t\tC: %s\n", question->choices->c);
      printf("\t\tD: %s\n", question->choices->d);
      printf("Answer: %s\n", question->answer);
      break;
    }
    printf("==============================\n");
  }
}
