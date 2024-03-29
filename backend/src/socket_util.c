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
    ssize_t amountRceived = recv(accepted_socket_FD, buffer, 4096, 0);
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
    message = "HTTP/1.0 200 OK";
    send(accepted_socket_FD, message, strlen(message), 0);
    close(accepted_socket_FD);
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
  //   pthread_create(&id, NULL, receive_and_process_incoming_data,
  //   &accepted_socket_FD);
  pthread_create(&id, NULL, handle_client, &accepted_socket_FD);
  free(pSocket);

  return NULL;
}

const char *get_file_extension(const char *file_name) {
  const char *dot = strrchr(file_name, '.');
  if (!dot || dot == file_name) {
    return "";
  }
  return dot + 1;
}

const char *get_mime_type(const char *file_ext) {
  if (strcasecmp(file_ext, "html") == 0 || strcasecmp(file_ext, "htm") == 0) {
    return "text/html";
  } else if (strcasecmp(file_ext, "txt") == 0) {
    return "text/plain";
  } else if (strcasecmp(file_ext, "jpg") == 0 ||
             strcasecmp(file_ext, "jpeg") == 0) {
    return "image/jpeg";
  } else if (strcasecmp(file_ext, "png") == 0) {
    return "image/png";
  } else {
    return "application/octet-stream";
  }
}

bool case_insensitive_compare(const char *s1, const char *s2) {
  while (*s1 && *s2) {
    if (tolower((unsigned char)*s1) != tolower((unsigned char)*s2)) {
      return false;
    }
    s1++;
    s2++;
  }
  return *s1 == *s2;
}

char *get_file_case_insensitive(const char *file_name) {
  DIR *dir = opendir(".");
  if (dir == NULL) {
    perror("opendir");
    return NULL;
  }

  struct dirent *entry;
  char *found_file_name = NULL;
  while ((entry = readdir(dir)) != NULL) {
    if (case_insensitive_compare(entry->d_name, file_name)) {
      found_file_name = entry->d_name;
      break;
    }
  }

  closedir(dir);
  return found_file_name;
}

char *url_decode(const char *src) {
  size_t src_len = strlen(src);
  char *decoded = malloc(src_len + 1);
  size_t decoded_len = 0;

  // decode %2x to hex
  for (size_t i = 0; i < src_len; i++) {
    if (src[i] == '%' && i + 2 < src_len) {
      unsigned int hex_val;
      sscanf(src + i + 1, "%2x", &hex_val);
      decoded[decoded_len++] = hex_val;
      i += 2;
    } else {
      decoded[decoded_len++] = src[i];
    }
  }

  // add null terminator
  decoded[decoded_len] = '\0';
  return decoded;
}

void build_http_response(char *response, size_t *response_len) {
  // build HTTP header
  // const char *mime_type = get_mime_type(file_ext);
  char *header = (char *)malloc(BUFFER_SIZE * sizeof(char));
  snprintf(header, BUFFER_SIZE,
           "HTTP/1.1 200 OK\r\n"
           "Content-Type: application/json\r\n"
           "Content-Length: 19\r\n"
           "\r\n"
           "{\"success\": \"true\"}");
  /*
    // 404 Not Found on error
    int file_fd = open(file_name, O_RDONLY);
    if (file_fd == -1) {
      snprintf(response, BUFFER_SIZE,
               "HTTP/1.1 404 Not Found\r\n"
               "Content-Type: text/plain\r\n"
               "\r\n"
               "404 Not Found");
      *response_len = strlen(response);
      return;
    }

    // get file size for Content-Length
    struct stat file_stat;
    fstat(file_fd, &file_stat);
    // off_t file_size = file_stat.st_size;
  */
  // copy header to response buffer
  *response_len = strlen(response);
  memcpy(response, header, strlen(header));
  *response_len += strlen(header);

  free(header);
  // close(file_fd);
}

void *handle_client(void *arg) {
  int client_fd = *((int *)arg);
  printf("Debug: client_fd: %d\n", client_fd);
  char *buffer = (char *)malloc(BUFFER_SIZE * sizeof(char));

  // receive request data from client and store into buffer
  ssize_t bytes_received = recv(client_fd, buffer, BUFFER_SIZE, 0);
  if (bytes_received > 0) {
    // check if request is GET
    // regex_t regex;
    // regcomp(&regex, "^GET /([^ ]*) HTTP/1", REG_EXTENDED);
    // regmatch_t matches[2];

    // if (regexec(&regex, buffer, 2, matches, 0) == 0) {
    // extract filename from request and decode URL
    //   buffer[matches[1].rm_eo] = '\0';
    // const char *url_encoded_file_name = buffer + matches[1].rm_so;
    // char *file_name = url_decode(url_encoded_file_name);

    // get file extension
    // char file_ext[32];
    // strcpy(file_ext, get_file_extension(file_name));

    // build HTTP response
    char *response = (char *)malloc(BUFFER_SIZE * 2 * sizeof(char));
    size_t response_len;
    printf("Debug: start building response\n");
    build_http_response(response, &response_len);

    // send HTTP response to client

    printf("Debug: start sending response\n");
    send(client_fd, response, response_len, 0);

    free(response);
    // free(file_name);
    // }
    // regfree(&regex);
  }
  close(client_fd);
  free(buffer);
  return NULL;
}