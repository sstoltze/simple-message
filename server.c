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

char *resize_buffer(char *buff, int new_size) {
  buff = (char *)realloc(buff, new_size*sizeof(char));
  return buff;
}

char *server_read(int connectionfd, char *buffer, int *size) {
  int valread = read(connectionfd, buffer, *size);
  while (valread == *size && valread != SO_ERROR) {
    buffer = resize_buffer(buffer, (*size)*2);
    valread += read(connectionfd, &buffer[*size], *size);
    (*size) *= 2;
  }
  if (valread != SO_ERROR) {
    buffer[valread] = '\0';
  }
  else {
    buffer = NULL;
  }
  return buffer;
}

void server_send(int *connectionfds, int num_connections, char *message) {
  size_t mess_len = strlen(message);
  for (int i=0;i < num_connections; i++) {
    send(connectionfds[i], message, mess_len, 0);
  }
}


int main() {
  struct sockaddr_in address;
  socklen_t addrlen = sizeof(address);
  int serverfd;
  int connectionfd;
  int valread;
  int opt = 1;
  int server_backlog = 5; // Backlog of received messages.

  int buffer_size = 1024;
  char *buffer = (char *)malloc(buffer_size*sizeof(char));
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
  buffer = server_read(connectionfd, buffer, &buffer_size);
  /* valread = read(connectionfd, buffer, 1023);
  if (valread != SO_ERROR) {
  buffer[valread] = '\0'; */
  if (buffer != NULL) {
    printf("%s\n", buffer);
    //send(connectionfd, message, strlen(message), 0);
    server_send(&connectionfd, 1, message);
    printf("Return message sent.\n");
  } else {
    printf("Error receiving message.\n");
  }

  close(connectionfd);
  close(serverfd);
  return 0;
}
