#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};
    char *response_header;
    char *response_body;
    FILE *fp;
    long file_size;

    // Create socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Set socket options
    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    // Set address and port
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Bind socket to address and port
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    while(1) {
        // Accept incoming connection
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0) {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        // Read request from client
        read(new_socket , buffer, BUFFER_SIZE);

        // Check if request is for /file.txt
        if (strncmp(buffer, "GET /file.txt", 13) == 0) {
            // Open file.txt and get its size
            fp = fopen("file.txt", "r");
            if (fp == NULL) {
                perror("fopen");
                exit(EXIT_FAILURE);
            }
            fseek(fp, 0L, SEEK_END);
            file_size = ftell(fp);
            fseek(fp, 0L, SEEK_SET);

            // Read contents of file into response_body
            response_body = malloc(file_size + 1);
            fread(response_body, file_size, 1, fp);
            fclose(fp);

            // Set response header
            response_header = "HTTP/1.1 200 OK\nContent-Type: text/plain\n\n";

            // Send response to client
            send(new_socket , response_header , strlen(response_header) , 0 );
            send(new_socket , response_body , file_size , 0 );
            free(response_body);
        } else {
            // Send 404 Not Found response for other requests
            response_header = "HTTP/1.1 404 Not Found\nContent-Type: text/html\n\n";
            response_body = "<html><body><h1>404 Not Found</h1></body></html>";
            send(new_socket , response_header , strlen(response_header) , 0 );
            send(new_socket , response_body , strlen(response_body) , 0 );
        }

        close(new_socket);
    }

    return 0;
}
