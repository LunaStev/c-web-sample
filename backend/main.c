#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>

#include "webserver.h"

#define PORT 8080

int main() {
    int server_fd, client_fd;
    struct sockaddr_in addr;
    char buffer[2048];

    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(PORT);

    bind(server_fd, (struct sockaddr *)&addr, sizeof(addr));
    listen(server_fd, 10);

    print_server_address(PORT);

    while (1) {
        client_fd = accept(server_fd, NULL, NULL);
        read(client_fd, buffer, sizeof(buffer) - 1);

        if (strncmp(buffer, "GET /api/hello ", 15) == 0) {
            send_response(client_fd, "200 OK", "text/plain", "Hello from raw C!");
        } else {
            char filepath[256] = "static";
            if (strncmp(buffer, "GET / ", 6) == 0)
                strcat(filepath, "/index.html");
            else {
                char *start = buffer + 4;
                char *end = strchr(start, ' ');
                *end = '\0';
                strcat(filepath, start);
            }
            send_file(client_fd, filepath);
        }

        close(client_fd);
    }

    return 0;
}
