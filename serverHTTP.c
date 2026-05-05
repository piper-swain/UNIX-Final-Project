#include <stdio.h>         // for printf, perror
#include <sys/socket.h>    // for socket functions
#include <unistd.h>        // for read, write, close
#include <stdlib.h>        // for exit, malloc, free
#include <netinet/in.h>    // for sockaddr_in
#include <string.h>        // for memset, strlen

#define MY_SERVER_PORT 9090     // Port number for the server
#define BUFFER_SIZE 1000000     // Buffer size for reading requests and file contents (1MB)

int main() {

    int server_fd, new_socket; 
    int addrlen = sizeof(struct sockaddr_in);  // Size of the address structure

    /////////////////////////////////////////////////
    //           Step 1: Create socket             //
    /////////////////////////////////////////////////
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {  // Create TCP socket
        perror("cannot create socket");
        exit(EXIT_FAILURE);
    }

    /////////////////////////////////////////////////
    // Step 2: Bind socket
    /////////////////////////////////////////////////
    int opt = 1;                                                        // Allow reuse of the address
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)); // Set socket options to allow address reuse

    struct sockaddr_in address;           // Structure to hold server address information
    memset(&address, 0, sizeof(address)); // Zero out the address structure

    address.sin_family = AF_INET;              // Use IPv4, which is the standard for internet communication
    address.sin_addr.s_addr = INADDR_ANY;      // Listen on all available interfaces 
    address.sin_port = htons(MY_SERVER_PORT);  // Convert port number to network byte order

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {   // Bind the socket to the specified port and address
        perror("bind failed"); 
        exit(EXIT_FAILURE);   
    }

    printf("Socket created and bound to port %d\n", MY_SERVER_PORT);  // Print a message indicating that the socket has been successfully created and bound

    /////////////////////////////////////////////////
    // Step 3: Listen
    /////////////////////////////////////////////////
    if (listen(server_fd, 3) < 0) {  // Start listening for incoming connections, with a backlog of 3
        perror("listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Listening...\n");

    /////////////////////////////
    // Main loop
    /////////////////////////////
    while (1) {  // Infinite loop to accept and handle incoming connections

        if ((new_socket = accept(server_fd,   // Accept an incoming connection
                (struct sockaddr *)&address,  // Fill in the client's address information
                (socklen_t*)&addrlen)) < 0) { // If accept fails, print an error message and continue to the next iteration of the loop
            perror("accept failed");
            continue;
        }

        printf("Client connected\n");

        /////////////////////////////////////////////////
        // Step 4: Read request
        /////////////////////////////////////////////////
        char buffer[BUFFER_SIZE] = {0};         // Buffer to hold the incoming request data, initialized to zero
        read(new_socket, buffer, BUFFER_SIZE);  // Read data from the client into the buffer

        printf("Request received:\n%s\n", buffer);  // Print the received request to the console for debugging purposes

        /////////////////////////////////////////////////
        // Step 5: Open HTML file
        /////////////////////////////////////////////////
        FILE *fp = fopen("index.html", "r");   // Open the "index.html" file for reading

        if (fp == NULL) {  // If the file cannot be opened, print an error message and send a 404 response to the client
            perror("Failed to open index.html");

            char *error_response =   // HTTP response for a 404 Not Found error, indicating that the requested file could not be found
                "HTTP/1.1 404 Not Found\r\n"
                "Content-Type: text/plain\r\n\r\n"
                "File not found";

            write(new_socket, error_response, strlen(error_response));  // Send the error response to the client
            close(new_socket);                                          // Close the client socket
            continue;                                                   // Move on to the next iteration of the loop to wait for another incoming connection
        }

        /////////////////////////////////////////////////
        // Step 6: Read file into buffer
        /////////////////////////////////////////////////
        fseek(fp, 0, SEEK_END);                     // Move the file pointer to the end of the file to determine its size
        long file_size = ftell(fp);                 // Get the current file pointer position, which is the size of the file in bytes
        rewind(fp);                                 // Move the file pointer back to the beginning of the file to prepare for reading its contents

        char *file_buffer = malloc(file_size);     // Allocate a buffer in memory to hold the contents of the file, with a size equal to the file size
        if (file_buffer == NULL) {                 // If memory allocation fails, print an error message, close the client socket, and continue to the next iteration of the loop
            perror("Failed to allocate memory for file buffer");
            close(new_socket);
            continue;
        }

        fread(file_buffer, 1, file_size, fp);  // Read the contents of the file into the allocated buffer, specifying the size of each element as 1 byte and the number of elements to read as the file size
        fclose(fp);                            // Close the file after reading its contents into the buffer

        /////////////////////////////////////////////////
        // Step 7: Send HTTP response
        /////////////////////////////////////////////////
        char header[256];  // Buffer to hold the HTTP response header

        sprintf(header,                        // Format the HTTP response header with the appropriate status line, content type, content length, and connection status
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/html\r\n"
            "Content-Length: %ld\r\n"
            "Connection: close\r\n\r\n",
            file_size
        );
        write(new_socket, header, strlen(header));    // Send the formatted HTTP response header to the client
        int total_sent = 0;                           // Variable to keep track of the total number of bytes sent to the client
        while (total_sent < file_size) {              // Loop to send the file contents to the client in chunks until the entire file has been sent
            int sent = write(new_socket, file_buffer + total_sent, file_size - total_sent);  // Send a chunk of the file contents to the client, starting from the current position in the buffer and specifying the remaining number of bytes to send
            if (sent <= 0) break;                     // If the write operation fails or the client disconnects, break out of the loop
            total_sent += sent;                       // Update the total number of bytes sent by adding the number of bytes just sent
        }

        /////////////////////////////////////////////////
        // Step 8: Cleanup
        /////////////////////////////////////////////////
        free(file_buffer);    // Free the allocated buffer that holds the file contents to prevent memory leaks
        shutdown(new_socket, SHUT_WR);  // tell browser that you're done sending data, but still allow it to read the response
        close(new_socket);              // Close the client socket to free up resources and allow the server to accept new connections

        printf("Response sent, connection closed\n\n");  // Print a message indicating that the response has been sent and the connection has been closed, then continue to the next iteration of the loop to wait for another incoming connection
    }

    close(server_fd);
    return 0;
}
