#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 8080
#define BUFFER_SIZE 1024
#define BACKLOG 5

int main() {
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    char buffer[BUFFER_SIZE];
    int addrlen = sizeof(client_addr);

    // Create socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }

    // Configure server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Bind socket to the address
    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("bind");
        close(server_fd);
        exit(1);
    }

    // Listen for incoming connections
    if (listen(server_fd, BACKLOG) == -1) {
        perror("listen");
        close(server_fd);
        exit(1);
    }

    printf("TCP Server listening on port %d...\n", PORT);

    // Accept a client connection
    if ((client_fd = accept(server_fd, (struct sockaddr *)&client_addr, (socklen_t *)&addrlen)) == -1) {
        perror("accept");
        close(server_fd);
        exit(1);
    }

    printf("Client connected!\n");

    while (1) {
        memset(buffer, 0, BUFFER_SIZE);

        // Receive data from client
        int valread = recv(client_fd, buffer, BUFFER_SIZE, 0);
        if (valread <= 0) {
            if (valread == 0) {
                printf("Client disconnected.\n");
            } else {
                perror("recv");
            }
            break;
        }
        buffer[valread] = '\0'; // Null-terminate the received string
        printf("Received from client: %s\n", buffer);

        // Check if client sent "exit"
        if (strcmp(buffer, "exit") == 0) {
            printf("Client requested to close connection. Exiting...\n");
            break;
        }

        // Reverse the string
        int n = strlen(buffer);
        for (int i = 0; i < n / 2; i++) {
            char temp = buffer[i];
            buffer[i] = buffer[n - i - 1];
            buffer[n - i - 1] = temp;
        }

        // Send the reversed string back to the client
        send(client_fd, buffer, strlen(buffer), 0);
        printf("Reversed string sent to client: %s\n", buffer);
    }

    close(client_fd);
    close(server_fd);
    return 0;
}
