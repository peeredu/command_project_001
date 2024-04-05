#include "socket_util.h"

static Server server_p;

Server server_init(int domain, int service, int protocol, unsigned long interface, int port, int backlog,
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

    if (bind(server.socket, (struct sockaddr *)&server.address, sizeof(server.address)) < 0) {
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
    int clientSocketFD = accept(server_socket_FD, (struct sockaddr *)&clientAddress, &clientAddressSize);

    struct AcceptedSocket *accepted_socket = (struct AcceptedSocket *)malloc(sizeof(struct AcceptedSocket));
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
        struct AcceptedSocket *clientSocket = accept_incoming_connection(server->socket);
        receive_and_process_incoming_data_on_separate_thread(clientSocket);
    }
}

void *receive_and_process_incoming_data_on_separate_thread(void *pSocket) {
    pthread_t id;
    int accepted_socket_FD = ((struct AcceptedSocket *)pSocket)->accepted_socket_FD;
    log_message2("Server", "Starting new thread for processing client connection...");
    pthread_create(&id, NULL, handle_client, &accepted_socket_FD);
    free(pSocket);

    return NULL;
}

void build_http_response(Request parsed_request, char *response, size_t *response_len) {
    log_message2("Server", "Building response...");
    printf("Debug: start building response\n");
    /*printf("Debug: PARSED request\n");
    printf("Debug: request method: %s\n", parsed_request.method);
    // printf("Debug: request path: %s\n", parsed_request.path);
    printf("Debug: request route_0: %s\n", parsed_request.route_0);
    printf("Debug: request route_1: %s\n", parsed_request.route_1);
    // printf("Debug: request route_2: %s\n", parsed_request.route_2);
    // printf("Debug: request http version: %s\n", parsed_request.http_version);
    printf("Debug: request length: %s\n", parsed_request.length);
    // printf("Debug: request body: %s\n", parsed_request.body);*/
    if (parsed_request.method == GET) {
        char content[500] = {0};

        // TEST ROUTE HARDCODED - REMOVE
        if (strcmp("items", parsed_request.route_0) == 0 && strlen(parsed_request.route_1) == 0) {
            strcat(content,
                   "{\"items\": [{\"id\": 1,\"name\":\"test\",\"price\": "
                   "100,\"quantity\": 10,\"in_stock\": 1 }, {\"id\": "
                   "2,\"name\":\"test2\",\"price\": 124,\"quantity\": "
                   "3,\"in_stock\": 1 }, {\"id\": 3,\"name\":\"test3\",\"price\": "
                   "223,\"quantity\": 2,\"in_stock\": 1 }]}");
        } else if (strcmp("items", parsed_request.route_0) == 0 && strlen(parsed_request.route_1) > 0) {
            // TEST DB CONNECTION TO GET DATA
            printf("Debug: DB connection...\n");
            MYSQL conn;

            int res = db_get_connect(&conn);
            if (res == 0) printf("Debug: DB connection success\n");
            Product product;
            // parse id from char* route_1
            int id = strtol(parsed_request.route_1, NULL, 10);
            if (db_get_product(&conn, &product, id) == 0) {
                printf("Debug: DB connection success\n");

                char *json = json_from_product(product);
                strcat(content, json);
                free(json);

            } else {
                printf("Debug: DB connection failed\n");
                strcat(content,
                       "{\"name\": \"\", \"price\": 0, \"quantity\": 0, "
                       "\"in_stock\": 0}");
            }
            // TEST WITHOUT DB CONNECTION
            /*Product product = {.id = 1,
                               .name = "Vooodkaaa",
                               .unit_price = 100,
                               .quantity = 10,
                               .active = 1};

            char *json = calloc(200, sizeof(char));
            json = json_from_product(product);
            strcat(content, json);
            free(json);*/

        } else {
            strcat(content, "{\"success\":\"true\"}");
        }

        // build response
        snprintf(response, MAX_RESPONSE_SIZE,
                 "HTTP/1.1 200 OK\r\n"
                 "Content-Type: application/json\r\n"
                 "Content-Length: %ld\r\n"
                 "Access-Control-Allow-Origin: *\r\n"
                 "\r\n"
                 "%s",
                 (long)strlen(content), content);

        *response_len = strlen(response);
    } else if (parsed_request.method == POST) {
        snprintf(response, MAX_RESPONSE_SIZE, DEFAUL_RESPONSE_TEXT);
    } else if (parsed_request.method == PUT) {
        snprintf(response, MAX_RESPONSE_SIZE, DEFAUL_RESPONSE_TEXT);
    } else if (parsed_request.method == PATCH) {
        snprintf(response, MAX_RESPONSE_SIZE, DEFAUL_RESPONSE_TEXT);
    } else if (parsed_request.method == DELETE) {
        snprintf(response, MAX_RESPONSE_SIZE, DEFAUL_RESPONSE_TEXT);
    }
}

int parse_http_request(char *request, Request *parsed_request) {
    log_message2("Server", "Parsing request...");
    log_message2("Request", request);
    printf("Debug: start parsing request...\n");

    int i = 0;
    char tmp_request[BUFFER_SIZE] = {0};
    memcpy(tmp_request, request, strlen(request));
    char *first_line = strtok(tmp_request, "\r\n");
    char *token = strtok(first_line, " ");

    parsed_request->method = GET;
    while (i < 3) {
        if (i == 0) {
            if (strcmp(token, "GET") == 0) parsed_request->method = GET;
            if (strcmp(token, "POST") == 0) parsed_request->method = POST;
            if (strcmp(token, "PUT") == 0) parsed_request->method = PUT;
            if (strcmp(token, "PATCH") == 0) parsed_request->method = PATCH;
            if (strcmp(token, "DELETE") == 0) parsed_request->method = DELETE;
        } else if (i == 1) {
            strcpy(parsed_request->path, token);
        } else if (i == 2) {
            strcpy(parsed_request->http_version, token);
        }
        i++;
        token = strtok(NULL, " ");
    }

    if (parsed_request->method == PUT || parsed_request->method == POST) {
        char tmp_content[BUFFER_SIZE] = {0};
        memcpy(tmp_content, request, strlen(request));
        char *current_content = strtok(tmp_content, "\r\n");
        while (current_content != NULL) {
            if ((strstr(current_content, "Content-Length"))) {
                strtok(current_content, " ");
                strcpy(parsed_request->length, strtok(NULL, " "));
            }
            current_content = strtok(NULL, "\r\n");
        }
    }

    // Parse routes from path
    char tmp_path[200] = {0};
    memcpy(tmp_path, parsed_request->path, strlen(parsed_request->path));
    char *route_token = strtok(tmp_path, "/");
    i = 0;

    while (route_token != NULL && i < 3) {
        if (i == 0) {
            strcpy(parsed_request->route_0, route_token);
        } else if (i == 1) {
            strcpy(parsed_request->route_1, route_token);
        } else if (i == 2) {
            strcpy(parsed_request->route_2, route_token);
        }
        i++;
        route_token = strtok(NULL, "/");
    }

    return 0;
}

void *handle_client(void *arg) {
    int client_fd = *((int *)arg);
    char buffer[BUFFER_SIZE] = {0};

    // receive request data from client and store into buffer
    ssize_t bytes_received = recv(client_fd, buffer, BUFFER_SIZE, 0);

    if (bytes_received > 0) {
        Request request_details;
        parse_http_request(buffer, &request_details);

        printf("Debug: end parsing request\n");
        printf("Debug: request method: %d\n", request_details.method);
        printf("Debug: request route_0: %s\n", request_details.route_0);
        printf("Debug: request route_1: %s\n", request_details.route_1);

        // build HTTP response
        char response[MAX_RESPONSE_SIZE] = {0};
        size_t response_len;

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
    printf(
        "\nOUCH, did you hit Ctrl-C?\n"
        "Do you really want to shutdown server? [y/n] ");
    c = getchar();
    if (c == 'y' || c == 'Y') {
        shutdown(server_p.socket, SHUT_RDWR);
        close_logger();
        exit(0);
    } else {
        signal(SIGINT, SIGINT_handler);
    }
    getchar();  // Get new line character
}