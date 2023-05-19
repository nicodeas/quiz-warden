#include "server.h"

int PORT;
char *LANGUAGE;
bool DEBUG = false;
char *QUESTION_FILE;

void usage(char *progname) {
  fprintf(stderr,
          "Usage: %s [options] language\n"
          "Options:\n"
          "  -p PORT	The port number to run this server\n\n"
          "Languages (case sensitive):\n"
          "  C\n"
          "  PYTHON\n",
          progname);
  exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]) {
  DEBUG = (getenv("DEBUG") != NULL);

  if (DEBUG) {
    printf("Debug: ON\n");
  }

  int opt;
  while ((opt = getopt(argc, argv, "p:")) != -1) {
    switch (opt) {
    case 'p':
      PORT = atoi(optarg);
      break;
    default:
      usage(argv[0]);
      break;
    }
  }

  // check if language is specified exactly once
  if (argv[optind] == NULL || argv[optind + 1] != NULL) {
    usage(argv[0]);
  }

  // config resources
  if ((strcmp(argv[optind], "C") == 0)) {
    QUESTION_FILE = CLANG_QUESTION_FILE;
  } else if (strcmp(argv[optind], "PYTHON") == 0) {
    QUESTION_FILE = PYTHON_QUESTION_FILE;
  } else {
    fprintf(stderr, "Invalid language specified\n");
    usage(argv[0]);
  }

  LANGUAGE = argv[optind];

  buildQuestionBank();

  if (DEBUG) {
    printQuestionBank();
    printf("Language has been set to %s\n", LANGUAGE);
  }

  int server_socket = createServer();
  runServer(server_socket);
}
