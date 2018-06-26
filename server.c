// TODO: http://www.microhowto.info/howto/listen_for_and_accept_tcp_connections_in_c.html
// Based on https://www.geeksforgeeks.org/socket-programming-cc/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/socket.h>
// https://linux.die.net/man/2/select
#include <sys/select.h>
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
  if (valread == 0) {
    return NULL;
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
  int valread;
  int opt = 1;
  int server_backlog = 5; // Backlog of received messages.

  int buffer_size = 1024;
  char *buffer = (char *)malloc(buffer_size*sizeof(char));
  int number_of_connections = 0;
  int max_connections = 1;
  int *connectionfds = (int *)malloc(max_connections*sizeof(int));
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
  address.sin_addr.s_addr = INADDR_ANY;
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
  //setsockopt(serverfd, SOL_SOCKET, O_NONBLOCK, &opt, sizeof(opt));

  // fd_set options
  fd_set rfds;
  struct timeval tv;
  int retval;

  while(1) {
    printf("Test.\n");
    // New connections
    FD_ZERO(&rfds);
    FD_SET(serverfd, &rfds);
    tv.tv_sec = 1; // Wait 1 sec
    tv.tv_usec = 0; // And 0 ms
    retval = select(FD_SETSIZE, &rfds, NULL, NULL, &tv);
    if (retval == -1) {
      printf("Error, server.\n");
      break;
    } else {
      if (FD_ISSET(serverfd, &rfds)) {
        printf("Server.\n");
        if ((connectionfds[number_of_connections] = accept(serverfd, (struct sockaddr *)&address,
                                                           &addrlen)) < 0) {
          perror("Error accepting.");
          exit(EXIT_FAILURE);
        }
        number_of_connections++;
        if (number_of_connections == max_connections) {
          max_connections *= 2;
          connectionfds = (int *)realloc(connectionfds, max_connections*sizeof(int));
        }
        printf("Connection established.\n");
      } else  {
        printf("Not set on server.\n");
      }
    }

    // Messages
    FD_ZERO(&rfds);
    for (int i=0;i<number_of_connections;i++) {
      FD_SET(connectionfds[i], &rfds);
    }
    tv.tv_sec = 1; // Wait 1 sec
    tv.tv_usec = 0; // And 0 ms
    retval = select(FD_SETSIZE, &rfds, NULL, NULL, &tv);
    if (retval == -1) {
      printf("Error, message.\n");
      break;
    } else {
      for (int i = 0;i<number_of_connections;i++) {
        if (FD_ISSET(connectionfds[i], &rfds)) {
          printf("Message received.\n");
          buffer = server_read(connectionfds[i], buffer, &buffer_size);
          if (buffer != NULL) {
            printf("Sending message: %s\n", buffer);
            server_send(connectionfds, number_of_connections, buffer);
          } else {
            printf("Error receiving message.\n");
            close(connectionfds[i]);
            // Make code to handle closing sockets...
            break;
          }
        }
      }
    }
  }
  // Cleanup
  for (int i=0; i<number_of_connections;i++) {
    close(connectionfds[i]);
  }
  free(connectionfds);
  close(serverfd);
  free(buffer);
  return 0;
}
