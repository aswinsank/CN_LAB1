#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

#define PORT 8080
#define BUFFER_SIZE 1024
#define IP "127.0.0.1"

int main() {
    int sockfd;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];
    int addrlen = sizeof(server_addr);

    // Create UDP socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }

    // Configure server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    if (inet_pton(AF_INET, IP, &server_addr.sin_addr) == -1) {
        perror("invalid address");
        close(sockfd);
        exit(1);
    }

    while (1) {
        memset(buffer, 0, BUFFER_SIZE);

        // Input message to send to the server
        printf("Enter a string to send to the server (type 'exit' to quit): ");
        fgets(buffer, BUFFER_SIZE, stdin);
        buffer[strcspn(buffer, "\n")] = 0; // Remove newline character

        // Check for "exit" command
        if (strcmp(buffer, "exit") == 0) {
            printf("Exiting...\n");
            sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr *)&server_addr, addrlen);
            break;
        }

        // Send the string to the server
        sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr *)&server_addr, addrlen);

        // Receive the reversed string from the server
        memset(buffer, 0, BUFFER_SIZE);
        int valread = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&server_addr, &addrlen);
        if (valread == -1) {
            perror("recvfrom");
            break;
        }
        buffer[valread] = '\0'; // Null-terminate the received string

        // Print the reversed string
        printf("Reversed string from server: %s\n", buffer);
    }

    close(sockfd);
    return 0;
}
