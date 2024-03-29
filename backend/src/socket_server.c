#include "socket_util.h"

int main(void) {
  // running server
  int server_socket_FD =
      create_tcp_ip_v4_socket(); // получаем сокет (дескриптор)
  int reuse = 1;
  if (setsockopt(server_socket_FD, SOL_SOCKET, SO_REUSEADDR,
                 (const char *)&reuse, sizeof(reuse)) < 0)
    perror("setsockopt(SO_REUSEADDR) failed");
  struct sockaddr_in *server_address = create_ip_v4_address(
      "", PORT); // задаём адрес и порт для входящих соединений, "" - это
                 // означает "все адреса"
  int result =
      bind(server_socket_FD, (struct sockaddr *)server_address,
           sizeof(*server_address)); // привязываем сокет к адресу и порту
  if (result == 0)
    printf("Debug: bind success\n");
  else if (result == -1) {
    printf("error: %s\n", strerror(errno));
    free(server_address);
    return 1;
  }

  int listen_result =
      listen(server_socket_FD,
             10); // запускаем прослушивание сокета с очередью на 10 запросов
  if (listen_result == 0)
    printf("Debug: port listening success\n");

  // start acepting incoming connections in separate threads
  start_accept_incoming_connections(server_socket_FD);

  shutdown(server_socket_FD, SHUT_RDWR);
  free(server_address);
  return 0;
}
