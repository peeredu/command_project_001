#include "socket_util.h"

int main(void) {
    log_message2("Server", "Initialising...");
    Server server =
        server_init(AF_INET, SOCK_STREAM, 0, INADDR_ANY, PORT, 10, start_accept_incoming_connections);
    signal(SIGINT, SIGINT_handler);
    log_message2("Server", "Initialised, launching...");
    server.launch(&server);
    log_message2("Server", "Shutting down...");
    shutdown(server.socket, SHUT_RDWR);
    close_logger();
    return 0;
}
