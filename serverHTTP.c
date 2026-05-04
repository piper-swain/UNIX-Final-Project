#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <time.h>

#define MY_SERVER_PORT 9090
#define BUFFER_SIZE 1000000

int main() {
    srand(time(NULL));

    int server_fd, new_socket;
    int addrlen = sizeof(struct sockaddr_in);

    /////////////////////////////////////////////////
    // Step 1: Create socket
    /////////////////////////////////////////////////
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("cannot create socket");
        exit(EXIT_FAILURE);
    }

    /////////////////////////////////////////////////
    // Step 2: Bind socket
    /////////////////////////////////////////////////
    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    struct sockaddr_in address;
    memset(&address, 0, sizeof(address));

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(MY_SERVER_PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    printf("Socket created and bound to port %d\n", MY_SERVER_PORT);

    /////////////////////////////////////////////////
    // Step 3: Listen
    /////////////////////////////////////////////////
    if (listen(server_fd, 3) < 0) {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Listening...\n");

    /////////////////////////////////////////////////
    // Main loop
    /////////////////////////////////////////////////
    while (1) {

        if ((new_socket = accept(server_fd,
                (struct sockaddr *)&address,
                (socklen_t*)&addrlen)) < 0) {
            perror("accept failed");
            continue;
        }

        printf("Client connected\n");

        /////////////////////////////////////////////////
        // Step 4: Read request
        /////////////////////////////////////////////////
        char buffer[BUFFER_SIZE] = {0};
        read(new_socket, buffer, BUFFER_SIZE);

        printf("Request received:\n%s\n", buffer);

        /////////////////////////////////////////////////
        // Step 5: Open HTML file
        /////////////////////////////////////////////////
        FILE *fp = fopen("index.html", "r");

        if (fp == NULL) {
            perror("Failed to open index.html");

            char *error_response =
                "HTTP/1.1 404 Not Found\r\n"
                "Content-Type: text/plain\r\n\r\n"
                "File not found";

            write(new_socket, error_response, strlen(error_response));
            close(new_socket);
            continue;
        }

        /////////////////////////////////////////////////
        // Step 6: Read file into buffer
        /////////////////////////////////////////////////
        fseek(fp, 0, SEEK_END);
        long file_size = ftell(fp);
        rewind(fp);

        char *file_buffer = malloc(file_size);

        fread(file_buffer, 1, file_size, fp);
        fclose(fp);

        /////////////////////////////////////////////////
        // Step 7: Send HTTP response
        /////////////////////////////////////////////////
        char header[256];

        sprintf(header,
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/html\r\n"
            "Content-Length: %ld\r\n"
            "Connection: close\r\n\r\n",
            file_size
        );
        write(new_socket, header, strlen(header));
        int total_sent = 0;
        while (total_sent < file_size) {
            int sent = write(new_socket, file_buffer + total_sent, file_size - total_sent);
            if (sent <= 0) break;
            total_sent += sent;
        }

        /////////////////////////////////////////////////
        // Step 8: Cleanup
        /////////////////////////////////////////////////
        free(file_buffer);
        shutdown(new_socket, SHUT_WR);  // tell browser "I'm DONE sending"
        close(new_socket);

        printf("Response sent, connection closed\n\n");
    }

    close(server_fd);
    return 0;
}
