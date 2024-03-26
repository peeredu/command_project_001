#include "socket_util.h"

int main(void) {
  int client_socket_FD = create_tcp_ip_v4_socket();

  char *ip = "127.0.0.1";
  struct sockaddr_in *server_address = create_ip_v4_address(ip, 2000);

  // connect
  int result = connect(client_socket_FD, (struct sockaddr *)server_address,
                       sizeof(*server_address));
  printf("connect result: %d\n", result); // result == 0 on success

  if (result == -1) {
    printf("error: %s\n", strerror(errno));
    free(server_address);
    return 1;
  }

  /*char *line = NULL;
  size_t lineSize = 0;
  char buffer[1024];
  while (true) {
      ssize_t charCount = getline(&line, &lineSize, stdin);
      if (charCount > 0) {
          if (strcmp(line, "exit\n") == 0) break;
          // ssize_t amountWasSent =
          send(client_socket_FD, line, charCount, 0);

          recv(client_socket_FD, buffer, 1024, 0);
          printf("Response: %s\n", buffer);
      }
  }*/

  // send request to test GET request
  char *message;
  message = "GET / HTTP/1.1\r\nHost:localhost\r\n\r\n";
  send(client_socket_FD, message, strlen(message), 0);
  char buffer[1024];
  recv(client_socket_FD, buffer, 1024, 0);
  printf("Response: %s\n", buffer);

  close(client_socket_FD);
  free(server_address);
  return 0;
}
