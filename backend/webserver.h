#ifndef WEBSERVER_H
#define WEBSERVER_H

void send_response(int client_fd, const char *status, const char *content_type, const char *body);
void send_file(int client_fd, const char *filepath);
void print_server_address(int port);
int is_wsl(void);

#endif // WEBSERVER_H
