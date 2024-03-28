#include "socket_util.h"

int create_tcp_ip_v4_socket(void) {
  int socketFD;
  socketFD = socket(AF_INET, SOCK_STREAM, 0);
  if (socketFD == -1) {
    printf("error: %s\n", strerror(errno));
    return -1;
  }
  return socketFD;
}

struct sockaddr_in *create_ip_v4_address(char *ip, int port) {
  struct sockaddr_in *address;
  address = (struct sockaddr_in *)malloc(sizeof(struct sockaddr_in));
  address->sin_family = AF_INET;
  address->sin_port = htons(port);
  if (strlen(ip) == 0)
    address->sin_addr.s_addr = htonl(INADDR_ANY);
  else
    inet_pton(AF_INET, ip, &address->sin_addr);
  return address;
}

struct AcceptedSocket *accept_incoming_connection(int server_socket_FD) {
  struct sockaddr_in clientAddress;
  unsigned int clientAddressSize = sizeof(struct sockaddr_in);
  int clientSocketFD = accept(
      server_socket_FD, (struct sockaddr *)&clientAddress, &clientAddressSize);

  struct AcceptedSocket *accepted_socket =
      (struct AcceptedSocket *)malloc(sizeof(struct AcceptedSocket));
  accepted_socket->accepted_socket_FD = clientSocketFD;
  accepted_socket->address = clientAddress;
  accepted_socket->error = 0;
  accepted_socket->accepted_successfully = clientSocketFD > 0;
  if (!accepted_socket->accepted_successfully) {
    accepted_socket->error = clientSocketFD;
  }
  return accepted_socket;
}

void *receive_and_process_incoming_data(void *socketFD) {
  char buffer[1024];

  int accepted_socket_FD = *(int *)socketFD;
  printf("Debug: thread entered\n");
  printf("Debug: accepted_socket_FD: %d\n", accepted_socket_FD);
  while (true) {
    ssize_t amountRceived = recv(accepted_socket_FD, buffer, 1024, 0);
    if (amountRceived > 0) {
      buffer[amountRceived] = '\0';
      printf("Response: %s\n", buffer);
    } else if (amountRceived == 0) {
      printf("client disconnected\n");
      break;
    } else {
      printf("error: %s\n", strerror(errno));
      break;
    }
    // send response back
    char *message;
    message = "{\" id \": \" Quality \"}";
    send(accepted_socket_FD, message, strlen(message), 0);
  }
  close(accepted_socket_FD);
  return NULL;
}

void start_accept_incoming_connections(int server_socket_FD) {
  while (true) {
    struct AcceptedSocket *clientSocket =
        accept_incoming_connection(server_socket_FD);
    receive_and_process_incoming_data_on_separate_thread(clientSocket);
  }
}

void *receive_and_process_incoming_data_on_separate_thread(void *pSocket) {
  pthread_t id;
  printf("Debug: thread started\n");
  int accepted_socket_FD =
      ((struct AcceptedSocket *)pSocket)->accepted_socket_FD;
  printf("Debug: accepted_socket_FD: %d\n", accepted_socket_FD);
  pthread_create(&id, NULL, receive_and_process_incoming_data,
                 &accepted_socket_FD);

  free(pSocket);

  return NULL;
}