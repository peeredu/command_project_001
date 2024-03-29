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
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>  // работает с сокетами
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>  // for close

#define BUFFER_SIZE 1048576
#define PORT 2000
struct AcceptedSocket {
    int accepted_socket_FD;
    struct sockaddr_in address;
    int error;
    bool accepted_successfully;
};

int create_tcp_ip_v4_socket(void);
struct sockaddr_in *create_ip_v4_address(char *ip, int port);
struct AcceptedSocket *accept_incoming_connection(int server_socket_FD);
void *receive_and_process_incoming_data(void *socketFD);
void start_accept_incoming_connections(int server_socket_FD);
void *receive_and_process_incoming_data_on_separate_thread(void *clientSocket);

const char *get_file_extension(const char *file_name);

const char *get_mime_type(const char *file_ext);

bool case_insensitive_compare(const char *s1, const char *s2);

char *get_file_case_insensitive(const char *file_name);

char *url_decode(const char *src);

void build_http_response(char *response, size_t *response_len);

void *handle_client(void *arg);
#endif  // _SOCKET_UTIL_H_