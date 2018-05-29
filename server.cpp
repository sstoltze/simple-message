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
    opt = 1;
    server_backlog = 5;
    addrlen = sizeof(address);
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
  }
  
private:
  int server_backlog;
  int opt;
  int serverfd;
  int connectionfd; // Should be array later.
  int number_connections;
  struct sockaddr_in address;
  socklen_t addrlen;
};

int main() {
  server s;
  s.setup(PORT);
}
