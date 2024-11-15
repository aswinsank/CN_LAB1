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

int main() {
    int sockfd;
    struct sockaddr_in server_addr, client_addr;
    char buffer[BUFFER_SIZE];
    int addrlen = sizeof(client_addr);

    // Create UDP socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }

    // Configure server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Bind socket to the server address
    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("bind");
        close(sockfd);
        exit(1);
    }

    printf("UDP Server listening on port %d...\n", PORT);

    while (1) {
        memset(buffer, 0, BUFFER_SIZE);

        // Receive data from client
        int valread = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&client_addr, &addrlen);
        if (valread == -1) {
            perror("recvfrom");
            break;
        }
        buffer[valread] = '\0'; // Null-terminate the received string

        printf("Received from client: %s\n", buffer);

        // Check if the client sent "exit"
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
        sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr *)&client_addr, addrlen);
        printf("Reversed string sent to client: %s\n", buffer);
    }

    close(sockfd);
    return 0;
}
