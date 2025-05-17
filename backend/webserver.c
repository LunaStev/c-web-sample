#include "webserver.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <netdb.h>
#include <ifaddrs.h>
#include <arpa/inet.h>

int is_wsl(void) {
    FILE *f = fopen("/proc/version", "r");
    if (!f) return 0;

    char buf[256];
    fread(buf, 1, sizeof(buf) - 1, f);
    fclose(f);

    return strstr(buf, "microsoft") != NULL || strstr(buf, "Microsoft") != NULL;
}

void print_server_address(int port) {
    struct ifaddrs *ifaddr, *ifa;
    char ip[INET_ADDRSTRLEN];

    if (getifaddrs(&ifaddr) == -1) {
        perror("getifaddrs");
        return;
    }

    int printed = 0;

    if (is_wsl()) {
        for (ifa = ifaddr; ifa; ifa = ifa->ifa_next) {
            if (!ifa->ifa_addr || ifa->ifa_addr->sa_family != AF_INET) continue;
            if (strcmp(ifa->ifa_name, "eth0") == 0) {
                struct sockaddr_in *sa = (struct sockaddr_in *) ifa->ifa_addr;
                inet_ntop(AF_INET, &sa->sin_addr, ip, sizeof(ip));
                printf("Server started on http://%s:%d\n", ip, port);
                printed = 1;
                break;
            }
        }
    } else {
        for (ifa = ifaddr; ifa; ifa = ifa->ifa_next) {
            if (!ifa->ifa_addr || ifa->ifa_addr->sa_family != AF_INET) continue;

            struct sockaddr_in *sa = (struct sockaddr_in *) ifa->ifa_addr;
            inet_ntop(AF_INET, &sa->sin_addr, ip, sizeof(ip));

            if (strcmp(ifa->ifa_name, "lo") == 0 || strstr(ifa->ifa_name, "eth") || strstr(ifa->ifa_name, "wlan")) {
                printf("Server started on http://%s:%d\n", ip, port);
                printed = 1;
            }
        }
    }

    if (!printed) {
        printf("Server started on http://localhost:%d (no IP found)\n", port);
    }

    freeifaddrs(ifaddr);
}

void send_response(int client_fd, const char *status, const char *content_type, const char *body) {
    char response[4096];
    sprintf(response,
            "HTTP/1.1 %s\r\n"
            "Content-Type: %s\r\n"
            "Content-Length: %ld\r\n"
            "Connection: close\r\n"
            "\r\n"
            "%s",
            status, content_type, strlen(body), body);
    write(client_fd, response, strlen(response));
}

void send_file(int client_fd, const char *filepath) {
    FILE *file = fopen(filepath, "rb");
    if (!file) {
        send_response(client_fd, "404 Not Found", "text/plain", "404 Not Found");
        return;
    }

    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    rewind(file);

    char *body = malloc(size);
    fread(body, 1, size, file);
    fclose(file);

    const char *content_type = strstr(filepath, ".html") ? "text/html" :
                               strstr(filepath, ".js") ? "application/javascript" : "text/plain";

    char header[1024];
    sprintf(header,
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: %s\r\n"
            "Content-Length: %ld\r\n"
            "Connection: close\r\n"
            "\r\n", content_type, size);

    write(client_fd, header, strlen(header));
    write(client_fd, body, size);
    free(body);
}
