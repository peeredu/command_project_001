#ifndef _SOCKET_UTIL_H_
#define _SOCKET_UTIL_H_

#include <arpa/inet.h>  // работает с IP-адресами
#include <ctype.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>  // работает с IP-адресами
#include <pthread.h>
#include <regex.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>  // работает с сокетами
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>  // for close

#include "../../data/src/db_models.h"
#include "../../data/src/db_api.h"
#include "common/logger.h"
#include "json_export.h"

#define MAX_REQUEST_ATTRIBUTE_SIZE 256
#define MAX_REQUEST_BODY_SIZE 1024
#define BUFFER_SIZE 2048
#define MAX_RESPONSE_SIZE 2048
#define MAX_HEADER_SIZE 1024
#define PORT 2000

typedef struct Server {
  int domain;
  int service;
  int protocol;
  unsigned long interface;
  int port;
  int backlog;

  struct sockaddr_in address;

  int socket;

  void (*launch)(struct Server *server);
} Server;

struct AcceptedSocket {
  int accepted_socket_FD;
  struct sockaddr_in address;
  int error;
  bool accepted_successfully;
};

typedef struct Request {
  char method[MAX_REQUEST_ATTRIBUTE_SIZE];
  char path[MAX_REQUEST_ATTRIBUTE_SIZE];
  char route_0[MAX_REQUEST_ATTRIBUTE_SIZE];
  char route_1[MAX_REQUEST_ATTRIBUTE_SIZE];
  char route_2[MAX_REQUEST_ATTRIBUTE_SIZE];
  char http_version[MAX_REQUEST_ATTRIBUTE_SIZE];
  char body[MAX_REQUEST_BODY_SIZE];;
  char length[MAX_REQUEST_ATTRIBUTE_SIZE];
} Request;

Server server_init(int domain, int service, int protocol,
                   unsigned long interface, int port, int backlog,
                   void (*launch)(Server *server));

struct AcceptedSocket *accept_incoming_connection(int server_socket_FD);
void start_accept_incoming_connections(Server *server);
void *receive_and_process_incoming_data_on_separate_thread(void *clientSocket);

int parse_http_request(char *request, Request *parsed_request);
void build_http_response(Request parsed_request, char *response,
                         size_t *response_len);

void *handle_client(void *arg);

void SIGINT_handler(int);

#endif  // _SOCKET_UTIL_H_