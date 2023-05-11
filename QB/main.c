#include "server.h"

int PORT;
char *LANGUAGE;
bool DEBUG = false;

void usage(char *progname) {
  fprintf(stderr,
          "Usage: %s [options] language\n"
          "Options:\n"
          "  -p PORT	The port number to run this server\n\n"
          "Languages (case sensitive):\n"
          "  C\n"
          "  Python\n",
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

  // check if language is valid
  if (strcmp(argv[optind], "C") != 0 && strcmp(argv[optind], "Python") != 0) {
    fprintf(stderr, "Invalid language specified\n");
    usage(argv[0]);
  }

  // set language
  int language_length = strlen(argv[optind]) + 1;

  if ((LANGUAGE = malloc(language_length * sizeof(char))) != NULL) {
    strncpy(LANGUAGE, argv[optind], language_length);
  } else {
    perror("malloc");
    exit(EXIT_FAILURE);
  }

  buildQuestionBank();
  if (DEBUG) {
    printf("Language has been set to %s\n", LANGUAGE);
    printQuestionBank();
  }

  int server_socket = createServer();
  runServer(server_socket);
}
