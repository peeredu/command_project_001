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
  //   pthread_create(&id, NULL, receive_and_process_incoming_data,
  //   &accepted_socket_FD);
  pthread_create(&id, NULL, handle_client, &accepted_socket_FD);
  free(pSocket);

  return NULL;
}

void build_http_response(char *response, size_t *response_len) {
  // build HTTP header
  char *header = (char *)malloc(BUFFER_SIZE * sizeof(char));
  char *contetnt = "{\"success\": \"true\"}";
  int contetnt_len = strlen(contetnt);

  snprintf(header, BUFFER_SIZE,
           "HTTP/1.1 200 OK\r\n"
           "Content-Type: application/json\r\n"
           "Content-Length: %d\r\n"
           "\r\n"
           "%s",
           contetnt_len, contetnt);

  // copy header to response buffer
  *response_len = strlen(response);
  memcpy(response, header, strlen(header));
  *response_len += strlen(header);

  free(header);
  // close(file_fd);
}

int parse_http_request(char *request, Request *parsed_request) {

  // TODO: parse request
  printf("Debug: start parsing request\n Request: %s\n", request);
  // Template
  parsed_request->method = "GET";
  parsed_request->path = "/";
  parsed_request->http_version = "HTTP/1.1";
  parsed_request->header = "Host:localhost\r\n\r\n";
  parsed_request->body = "";
  printf("Debug: end parsing request\n");

  return 0;
}

void *handle_client(void *arg) {
  int client_fd = *((int *)arg);
  printf("Debug: client_fd: %d\n", client_fd);
  char *buffer = (char *)malloc(BUFFER_SIZE * sizeof(char));

  // receive request data from client and store into buffer
  ssize_t bytes_received = recv(client_fd, buffer, BUFFER_SIZE, 0);
  if (bytes_received > 0) {
    // TODO: parse request data and extract path
    Request parsed_request;
    parse_http_request(buffer, &parsed_request);

    // build HTTP response
    char *response = (char *)malloc(BUFFER_SIZE * 2 * sizeof(char));
    size_t response_len;
    printf("Debug: start building response\n");

    // TODO: update build_http_response(response, &response_len); for different
    // requests
    build_http_response(response, &response_len);

    // send HTTP response to client

    printf("Debug: start sending response\n");
    send(client_fd, response, response_len, 0);

    free(response);

    // regfree(&regex);
  }
  close(client_fd);
  free(buffer);
  return NULL;
}