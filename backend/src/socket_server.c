#include "socket_util.h"

int main(void) {

  Server server = server_init(AF_INET, SOCK_STREAM, 0, INADDR_ANY, PORT, 10,
                              start_accept_incoming_connections);

  server.launch(&server);
  shutdown(server.socket, SHUT_RDWR);

  return 0;
}
