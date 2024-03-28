#ifndef _SOCKET_UTIL_H_
#define _SOCKET_UTIL_H_

#include <arpa/inet.h>  // работает с IP-адресами
#include <errno.h>
#include <netinet/in.h>  // работает с IP-адресами
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>  // работает с сокетами
#include <unistd.h>      // for close
#include <pthread.h>


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
#endif  // _SOCKET_UTIL_H_