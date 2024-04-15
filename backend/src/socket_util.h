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

#include "../../data/src/db_api.h"
#include "../../data/src/db_models.h"
#include "common/logger.h"
#include "json_export.h"
#include "json_parser/jsense.h"

#define MAX_REQUEST_ATTRIBUTE_SIZE 256
#define MAX_REQUEST_BODY_SIZE 1024
#define BUFFER_SIZE 2048
#define MAX_RESPONSE_SIZE 2048
#define MAX_HEADER_SIZE 1024
#define DEFAULT_PORT 2000
#define DEFAUL_RESPONSE_TEXT                                                \
    "HTTP/1.1 200 OK\r\nContent-Type: application/json\r\nContent-Length: " \
    "18\r\nAccess-Control-Allow-Origin: *\r\n\r\n{\"success\":\"true\"}"

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

typedef enum HttpRequestMethod {
    GET = 0,
    POST = 1,
    PUT = 2,
    PATCH = 3,
    DELETE = 4,
    OPTIONS = 5
} HttpRequestMethod;

typedef struct Request {
    HttpRequestMethod method;
    char path[MAX_REQUEST_ATTRIBUTE_SIZE];
    char route_0[MAX_REQUEST_ATTRIBUTE_SIZE];
    char route_1[MAX_REQUEST_ATTRIBUTE_SIZE];
    char route_2[MAX_REQUEST_ATTRIBUTE_SIZE];
    char http_version[MAX_REQUEST_ATTRIBUTE_SIZE];
    char body[MAX_REQUEST_BODY_SIZE];
    char origin[MAX_REQUEST_ATTRIBUTE_SIZE];
    char length[MAX_REQUEST_ATTRIBUTE_SIZE];
} Request;

Server server_init(int domain, int service, int protocol, unsigned long interface, int port, int backlog,
                   void (*launch)(Server *server));

struct AcceptedSocket *accept_incoming_connection(int server_socket_FD);
void start_accept_incoming_connections(Server *server);
void *receive_and_process_incoming_data_on_separate_thread(void *clientSocket);

int parse_http_request(char *request, Request *parsed_request);
void build_http_response(Request parsed_request, char *response, size_t *response_len);

void *handle_client(void *arg);

void SIGINT_handler(int);

void process_route_get_items(char *content);
void process_route_get_item_by_id(Request parsed_request, char *content);
void process_route_get_order_by_id(Request parsed_request, char *content);
void write_json_in_file(char *json);
void parse_json(ParsedProduct *parsed_product, HttpRequestMethod method);

#endif  // _SOCKET_UTIL_H_