#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <netinet/in.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <unistd.h>

char response[] =
    "HTTP/1.1 200 OK\n"
    "Content-Type: text/html\n"
    "\n"
    "<html>"
      "<head>"
        "<title>PHP is the BEST!!1</title>"
      "</head>"
      "<body>"
        "<h1>PHP is the BEST!!1</h1>"
      "</body>"
    "</html>";

int main(int argc, char *argv[])
{
    if (argc < 2) {
        fprintf(stderr, "nodec <port>\n");
        exit(1);
    }

    uint16_t port = 0;

    {
        char *endptr;
        port = (uint16_t) strtoul(argv[1], &endptr, 10);

        if (endptr == argv[1]) {
            fprintf(stderr, "%s is not a port number\n", argv[1]);
            exit(1);
        }
    }

    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        fprintf(stderr, "Could not create socket epicly: %s\n", strerror(errno));
        exit(1);
    }

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_port = htons(port);

    ssize_t err = bind(server_fd, (struct sockaddr*) &server_addr, sizeof(server_addr));
    if (err != 0) {
        fprintf(stderr, "Could not bind socket epicly: %s\n", strerror(errno));
        exit(1);
    }

    err = listen(server_fd, 69);
    if (err != 0) {
        fprintf(stderr, "Could not listen to socket, it's too quiet: %s\n", strerror(errno));
        exit(1);
    }

    for (;;) {
        struct sockaddr_in client_addr;
        socklen_t client_addrlen = 0;
        int client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_addrlen);
        if (client_fd < 0) {
            fprintf(stderr, "Could not accept connection. This is unacceptable! %s\n", strerror(errno));
            exit(1);
        }

        assert(client_addrlen == sizeof(client_addr));

        err = write(client_fd, response, sizeof(response));
        if (err < 0) {
            fprintf(stderr, "Could not send data: %s\n", strerror(errno));
        }

        err = close(client_fd);
        if (err < 0) {
            fprintf(stderr, "Could not close client connection: %s\n", strerror(errno));
        }
    }

    return 0;
}