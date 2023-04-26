#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#define PORT "8080"
#define BACKLOG 10

void *get_in_addr(struct sockaddr *sa) {
  if (sa->sa_family == AF_INET) {
    return &(((struct sockaddr_in *)sa)->sin_addr);
  }
  return &(((struct sockaddr_in6 *)sa)->sin6_addr);
}

int main(void) {
  int sockfd, new_fd;
  struct addrinfo hints, *servinfo, *p;
  struct sockaddr_storage their_addr; // connector's address information
  socklen_t sin_size;
  struct sigaction sa;
  int yes = 1;
  char s[INET6_ADDRSTRLEN];
  int rv;
  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC; // used to specify the address family that should
                               // be used when creating a socket
  // AF_UNSPEC means that the socket can use either IPv4 or IPv6; unspecified
  hints.ai_socktype = SOCK_STREAM; // specify whether you want ot use sock
                                   // stream or sock datagram
  hints.ai_flags = AI_PASSIVE; // when the AI_PASSIVE flag is specified and node
                               // is NULL, returned socket addresses will
  // be suitable for binding to a socket that will accept connections

  if ((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0) {
    perror("getaddrinfo");
  }
  // loop through all the results and bind to the first we can
  for (p = servinfo; p != NULL; p = p->ai_next) {
    if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
      perror("server: socket");
      continue;
    }

    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
      perror("setsockopt");
      exit(1);
    }

    if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
      close(sockfd);
      perror("server: bind");
      continue;
    }

    break;
  }
  if (p == NULL) {
    printf("server failed to bind\n");
    exit(1);
  }
  if (listen(sockfd, BACKLOG) == -1) {
    perror("listen");
    exit(1);
  }
  printf("Server is up and waiting for connections...\n");

  while (1) {
    sin_size = sizeof their_addr;
    new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
    if (new_fd == -1) {
      perror("accept");
      continue;
    }
    inet_ntop(their_addr.ss_family, get_in_addr((struct sockaddr *)&their_addr),
              s, sizeof s);
    printf("server got connection from %s\n", s);
    if (!fork()) {
      close(sockfd);
      if (send(new_fd, "Hello, world!", 13, 0) == -1) {
        perror("send");
      }
      close(new_fd);
      exit(0);
    }
    close(new_fd);
  }
  return 0;
}
