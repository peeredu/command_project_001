#include "socket_util.h"

static Server server_p;

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
  server_p = server;
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

void build_http_response(Request parsed_request, char *response,
                         size_t *response_len) {
  // build HTTP header
  char header[MAX_RESPONSE_SIZE] = {0};
  // TEST ROUTE HARDCODED - REMOVE
  char content[5000] = {0};
  if (strcmp("/items", parsed_request.path) == 0) {
    strcat(content,
           "{\"items\": [{\"id\": 1,\"name\":\"test\",\"price\": "
           "100,\"quantity\": 10,\"in_stock\": 1 }, {\"id\": "
           "2,\"name\":\"test2\",\"price\": 124,\"quantity\": "
           "3,\"in_stock\": 1 }, {\"id\": 3,\"name\":\"test3\",\"price\": "
           "223,\"quantity\": 2,\"in_stock\": 1 }]}");
  } else {
    strcat(content, "{\"success\":\"true\"}");
  }
  int content_len = strlen(content);

  snprintf(header, MAX_RESPONSE_SIZE,
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
  int i = 0;
  char tmp_request[BUFFER_SIZE] = {0};
  memcpy(tmp_request, request, strlen(request));
  char *first_line = strtok(tmp_request, "\r\n");
  char *token = strtok(first_line, " ");

  while (i < 3) {
    if (i == 0) {
      parsed_request->method = token;
    } else if (i == 1) {
      parsed_request->path = token;
    } else if (i == 2) {
      parsed_request->http_version = token;
    }
    i++;
    token = strtok(NULL, " ");
  }

  if (strcmp(parsed_request->method, "PUT") ||
      strcmp(parsed_request->method, "POST")) {
    char tmp_content[BUFFER_SIZE] = {0};
    memcpy(tmp_content, request, strlen(request));
    char *current_content = strtok(tmp_content, "\r\n");
    while (current_content != NULL) {
      if ((strstr(current_content, "Content-Length"))) {
        token = strtok(current_content, " ");
        token = strtok(NULL, " ");
        parsed_request->length = token;
      }
      current_content = strtok(NULL, "\r\n");
    }
  }
  /* TODO: parse path into array 'parsed_request->routes[]' of strings delimited
   * by '/' */
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

    printf("Debug: end parsing request\n");
    printf("Debug: request method: %s\n", request_details.method);
    printf("Debug: request path: %s\n", request_details.path);
    printf("Debug: request http version: %s\n", request_details.http_version);
    printf("Debug: request length: %s\n", request_details.length);
    printf("Debug: request body: %s\n", request_details.body);

    // build HTTP response
    char response[MAX_RESPONSE_SIZE] = {0};
    size_t response_len;

    log_message2("Server", "Building response...");
    printf("Debug: start building response\n");

    // TODO: update build_http_response(response, &response_len); for different
    // requests
    build_http_response(request_details, response, &response_len);

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

void SIGINT_handler(int sig) {
  char c;

  signal(sig, SIG_IGN);
  printf("\nOUCH, did you hit Ctrl-C?\n"
         "Do you really want to shutdown server? [y/n] ");
  c = getchar();
  if (c == 'y' || c == 'Y') {
    shutdown(server_p.socket, SHUT_RDWR);
    close_logger();
    exit(0);
  } else {
    signal(SIGINT, SIGINT_handler);
  }
  getchar(); // Get new line character
}