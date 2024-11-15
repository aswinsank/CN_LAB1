#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

int isPalindrome(char *str) {
    int len = strlen(str);
    for (int i = 0; i < len / 2; i++) {
        if (str[i] != str[len - i - 1]) {
            return 0;
        }
    }
    return 1;
}

int main() {
    int serverSocket, clientSocket;
    struct sockaddr_in serverAddr, clientAddr;
    char buffer[1024];
    socklen_t clientAddrLen = sizeof(clientAddr);

    // Create the socket
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Define the server address
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    // Bind the socket to the address
    if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("Binding failed");
        close(serverSocket);
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections (backlog size of 3)
    if (listen(serverSocket, 3) < 0) {
        perror("Listen failed");
        close(serverSocket);
        exit(EXIT_FAILURE);
    }
    printf("Server listening on port 8080...\n");

    // Accept client connection
    clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddr, &clientAddrLen);
    if (clientSocket < 0) {
        perror("Accept failed");
        close(serverSocket);
        exit(EXIT_FAILURE);
    }

    printf("Client connected: %s\n", inet_ntoa(clientAddr.sin_addr));

    // Handle communication with the client
    while (1) {
        memset(buffer, 0, sizeof(buffer));

        // Receive the message from the client
        int bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesRead <= 0) {
            if (bytesRead == 0) {
                printf("Client disconnected\n");
            } else {
                perror("Recv failed");
            }
            break;
        }

        // Check if the string is a palindrome
        char *response = isPalindrome(buffer) ? "YES" : "NO";

        // Send the response back to the client
        send(clientSocket, response, strlen(response), 0);
    }

    // Close the client and server sockets
    close(clientSocket);
    close(serverSocket);

    return 0;
}
