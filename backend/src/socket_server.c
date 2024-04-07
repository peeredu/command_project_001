#include "socket_util.h"

int main(int argc, char* argv[]) {
    // get port in this priority: command line argument, environment variable, default value
    int port = 0;
    char* port_env_var = getenv("SHOP_BACKEND_PORT");
    if (port_env_var != NULL) port = atoi(port_env_var);
    if (argc == 2) port = atoi(argv[1]);
    port = (port > 1000) ? port : DEFAULT_PORT;

    log_message2("Server", "Initialising...");
    Server server =
        server_init(AF_INET, SOCK_STREAM, 0, INADDR_ANY, port, 10, start_accept_incoming_connections);
    signal(SIGINT, SIGINT_handler);
    log_message2("Server", "Initialised, launching...");
    server.launch(&server);
    log_message2("Server", "Shutting down...");
    shutdown(server.socket, SHUT_RDWR);
    close_logger();
    return 0;
}
