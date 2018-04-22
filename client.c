// TODO: http://www.microhowto.info/howto/listen_for_and_accept_tcp_connections_in_c.html
// Based on https://www.geeksforgeeks.org/socket-programming-cc/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

// TODO: Make configurable
#define PORT 8080

int main() {
  struct sockaddr_in address;
  struct sockaddr_in server_addr;
  int connectionfd;
  int valread;

  char buffer[1024];
  char *message = "Hello from client.";

  if ((connectionfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("Error creating socket.");
    exit(EXIT_FAILURE);
  }

  // ?
  memset(&server_addr, '0', sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(PORT);

  // Convert IPv4 and IPv6 from text to binary
  if (inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr) <= 0) {
    perror("Invalid address.");
    exit(EXIT_FAILURE);
  }

  if (connect(connectionfd, (struct sockaddr *)&server_addr,
              sizeof(server_addr)) < 0) {
    perror("Error connecting.");
    exit(EXIT_FAILURE);
  }

  send(connectionfd, message, strlen(message), 0);
  printf("Message sent.\n");
  valread = read(connectionfd, buffer, 1024);
  printf("%s\n", buffer);

  close(connectionfd);
    
  return 0;
}
