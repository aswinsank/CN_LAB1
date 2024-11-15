#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 8080
#define BUFFER_SIZE 1024
#define IP "127.0.0.1"  // Server IP

int main() {
    int sockfd;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};

    // Create TCP socket
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }

    // Setup server address
    address.sin_family = AF_INET;
    address.sin_port = htons(PORT);
    if (inet_pton(AF_INET, IP, &address.sin_addr) <= 0) {
        perror("invalid address");
        exit(1);
    }

    // Connect to the server
    if (connect(sockfd, (struct sockaddr *)&address, addrlen) == -1) {
        perror("connect");
        exit(1);
    }

    while (1) {
        memset(buffer, 0, BUFFER_SIZE);
        printf("\nEnter a number to check if it's an Armstrong number (type 'exit' to quit): ");
        fgets(buffer, BUFFER_SIZE, stdin);
        buffer[strcspn(buffer, "\n")] = 0;  // Remove newline from input

        // Exit condition
        if (strcmp(buffer, "exit") == 0) {
            printf("\nExiting...\n");
            break;
        }

        // Send the number to the server
        send(sockfd, buffer, strlen(buffer), 0);

        // Receive the result from the server
        memset(buffer, 0, BUFFER_SIZE);
        int valread = recv(sockfd, buffer, BUFFER_SIZE, 0);
        if (valread == -1) {
            perror("recv");
            break;
        }

        printf("\nServer: %s\n", buffer);
    }

    close(sockfd);
    return 0;
}
