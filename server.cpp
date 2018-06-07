#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

// TODO: Make configurable
#define PORT 8080

using namespace std;

class server {
public:
  server() {
    buffer = NULL;
    resize_buffer(1024);
    opt = 1;
    server_backlog = 5;
    addrlen = sizeof(address);
    number_connections = 0;
    connectionfd = 0;
  }

  ~server() {
    if (connectionfd != 0) {
      close(connectionfd);
    }
    free(buffer);
  }

  bool setup(int port) {
    serverfd = socket(AF_INET, SOCK_STREAM, 0);
    if (serverfd == 0) {
      return false;
    }
    if (setsockopt(serverfd, SOL_SOCKET, SO_REUSEADDR,
                   &opt, sizeof(opt))) {
      return false;
    }
    // ???
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);
    if (bind(serverfd, (struct sockaddr *) &address,
             sizeof(address)) < 0) {
      return false;
    }
    printf("Done.\n");
    return true;
  }

  bool server_listen() {
    if (listen(serverfd, server_backlog) < 0) {
      return false;
    }
    return true;
  }

  bool server_accept() {
    connectionfd = accept(serverfd, (struct sockaddr *) &address,
                          &addrlen);
    if (connectionfd < 0) {
      return false;
    }
    printf("Connection established.\n");
    return true;
  }

  char *server_read() {
    int valread = read(connectionfd, buffer, buffer_size);
    if (valread != SO_ERROR) {
      while (valread == buffer_size) {
        int original_size = buffer_size;
        resize_buffer(2*buffer_size);
        valread += read(connectionfd, &buffer[original_size], original_size);
      }
      buffer[valread] = '\0';
      printf("%s\n", buffer);
      return buffer;
    }
  }

  bool server_send(char *message) {
    send(connectionfd, message, strlen(message), 0);
    return true;
  }

private:
  char *buffer;
  int buffer_size;
  int server_backlog;
  int opt;
  int serverfd;
  int connectionfd; // Should be array later.
  int number_connections;
  struct sockaddr_in address;
  socklen_t addrlen;

  bool resize_buffer(int new_size) {
    buffer_size = new_size;
    buffer = (char *)realloc(buffer, buffer_size*sizeof(char));
    return (! (buffer==NULL));
  }
};

int main() {
  server s;
  s.setup(PORT);
  s.server_listen();
  s.server_accept();
  char *buff = s.server_read();
  s.server_send(buff);
}
