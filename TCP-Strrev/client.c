#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 8080
#define BUFFER_SIZE 1024
#define IP "127.0.0.1"

int main() {
    int sockfd;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];

    // Create socket
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }

    // Configure server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    if (inet_pton(AF_INET, IP, &server_addr.sin_addr) <= 0) {
        perror("invalid address");
        close(sockfd);
        exit(1);
    }

    // Connect to the server
    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("connect");
        close(sockfd);
        exit(1);
    }

    printf("Connected to the server!\n");

    while (1) {
        memset(buffer, 0, BUFFER_SIZE);

        // Input message to send to the server
        printf("Enter a string to send to the server (type 'exit' to quit): ");
        fgets(buffer, BUFFER_SIZE, stdin);
        buffer[strcspn(buffer, "\n")] = 0; // Remove newline character

        // Check for "exit" command
        if (strcmp(buffer, "exit") == 0) {
            printf("Exiting...\n");
            send(sockfd, buffer, strlen(buffer), 0);
            break;
        }

        // Send the string to the server
        send(sockfd, buffer, strlen(buffer), 0);

        // Receive the reversed string from the server
        memset(buffer, 0, BUFFER_SIZE);
        int valread = recv(sockfd, buffer, BUFFER_SIZE, 0);
        if (valread <= 0) {
            if (valread == 0) {
                printf("Server closed the connection.\n");
            } else {
                perror("recv");
            }
            break;
        }
        buffer[valread] = '\0'; // Null-terminate the received string

        // Print the reversed string
        printf("Reversed string from server: %s\n", buffer);
    }

    close(sockfd);
    return 0;
}
