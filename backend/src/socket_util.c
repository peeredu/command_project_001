#include "socket_util.h"

Server server_init(int domain, int service, int protocol,
                   unsigned long interface, int port, int backlog,
                   void (*launch)(Server *server)) {
  Server server;

  server.domain = domain;
  server.service = service;
  server.protocol = protocol;
  server.interface = interface;
  server.port = port;
  server.backlog = backlog;

  server.address.sin_family = domain;
  server.address.sin_port = htons(port);
  server.address.sin_addr.s_addr = htonl(interface);

  server.socket = socket(domain, service, protocol);
  if (server.socket == 0) {
    log_message2("Error", "Failed to create TCP/IP v4 socket...");
    perror("Failed to create TCP/IP v4 socket...");
    exit(EXIT_FAILURE);
  }

  if (bind(server.socket, (struct sockaddr *)&server.address,
           sizeof(server.address)) < 0) {

    log_message2("Error", "Failed to bind socket...");
    perror("Failed to bind socket...");
    exit(EXIT_FAILURE);
  }

  if (listen(server.socket, server.backlog) < 0) {
    log_message2("Error:", "Failed to start listening...");
    perror("Failed to start listening...");
    exit(EXIT_FAILURE);
  }

  server.launch = launch;

  return server;
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
  log_message2("Server", "Connection accepted.");
  return accepted_socket;
}

void start_accept_incoming_connections(Server *server) {
  while (true) {
    struct AcceptedSocket *clientSocket =
        accept_incoming_connection(server->socket);
    receive_and_process_incoming_data_on_separate_thread(clientSocket);
  }
}

void *receive_and_process_incoming_data_on_separate_thread(void *pSocket) {
  pthread_t id;
  int accepted_socket_FD =
      ((struct AcceptedSocket *)pSocket)->accepted_socket_FD;
  log_message2("Server",
               "Starting new thread for processing client connection...");
  pthread_create(&id, NULL, handle_client, &accepted_socket_FD);
  free(pSocket);

  return NULL;
}

void build_http_response(char *response, size_t *response_len) {
  // build HTTP header
  char header[MAX_HEADER_SIZE] = {0};
  char *content = "{\"success\": \"true\"}";
  int content_len = strlen(content);

  snprintf(header, MAX_HEADER_SIZE,
           "HTTP/1.1 200 OK\r\n"
           "Content-Type: application/json\r\n"
           "Content-Length: %d\r\n"
           "\r\n"
           "%s",
           content_len, content);

  // copy header to response buffer
  memcpy(response, header, strlen(header));
  *response_len = strlen(response);
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
  char buffer[BUFFER_SIZE] = {0};

  // receive request data from client and store into buffer
  ssize_t bytes_received = recv(client_fd, buffer, BUFFER_SIZE, 0);
  if (bytes_received > 0) {
    // TODO: parse request data and extract path
    Request request_details;

    log_message2("Server", "Parsing request...");
    log_message2("Request", buffer);

    printf("Debug: start parsing request...\n");

    parse_http_request(buffer, &request_details);

    // build HTTP response
    char response[MAX_RESPONSE_SIZE] = {0};
    size_t response_len;

    log_message2("Server", "Building response...");
    printf("Debug: start building response\n");

    // TODO: update build_http_response(response, &response_len); for different
    // requests
    build_http_response(response, &response_len);

    // send HTTP response to client
    log_message2("Server", "Sending response...");
    log_message2("Response", response);
    printf("Debug: start sending response\n");

    send(client_fd, response, response_len, 0);
  }
  close(client_fd);
  log_message2("Server", "Connection closed.");
  close_logger();
  return NULL;
}