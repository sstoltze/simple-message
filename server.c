
// TODO: http://www.microhowto.info/howto/listen_for_and_accept_tcp_connections_in_c.html
// Based on https://www.geeksforgeeks.org/socket-programming-cc/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

// TODO: Make configurable
#define PORT 8080

int main() {
  struct sockaddr_in address;
  socklen_t addrlen = sizeof(address);
  int serverfd;
  int connectionfd;
  int valread;
  int opt = 1;
  int server_backlog = 5; // Backlog of received messages.

  char buffer[1024];
  char *message = "Hello from C server.";

  if ((serverfd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
    perror("Error creating socket.");
    exit(EXIT_FAILURE);
  }

  // Socket options. Optional, but useful
  if (setsockopt(serverfd, SOL_SOCKET, SO_REUSEADDR,
                 &opt, sizeof(opt))) {
    perror("Error setting socket options.");
    exit(EXIT_FAILURE);
  }

  // ?
  address.sin_family = AF_INET;
  // ?
  address.sin_addr.s_addr = INADDR_ANY;
  // ?
  address.sin_port = htons(PORT);

  // Attach socket
  printf("Before bind.\n");
  if (bind(serverfd, (struct sockaddr *)&address,
           sizeof(address)) < 0) {
    perror("Error binding socket.");
    exit(EXIT_FAILURE);
  }

  printf("Before listen.\n");
  if (listen(serverfd, server_backlog) < 0) {
    perror("Error listening.");
    exit(EXIT_FAILURE);
  }
  printf("Before accept.\n");
  if ((connectionfd = accept(serverfd, (struct sockaddr *)&address, 
                            &addrlen)) < 0) {
    perror("Error accepting.");
    exit(EXIT_FAILURE);
  }

  printf("Connection established. Message received:\n");
  valread = read(connectionfd, buffer, 1024);
  printf("%s\n", buffer);
  send(connectionfd, message, strlen(message), 0);
  printf("Return message sent.\n");
  
  close(connectionfd);
  close(serverfd);
  return 0;
}







