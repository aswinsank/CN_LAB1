#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

int main()
{
    int clientSocket;
    struct sockaddr_in serverAddr;
    char buffer[1024];

    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket < 0)
    {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(clientSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
    {
        perror("Connection failed");
        close(clientSocket);
        exit(EXIT_FAILURE);
    }

    printf("Enter a numeric string: ");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';

    send(clientSocket, buffer, strlen(buffer), 0);

    int recvLen = recv(clientSocket, buffer, sizeof(buffer), 0);
    if (recvLen > 0)
    {
        buffer[recvLen] = '\0';
        printf("Sum of digits: %s\n", buffer);
    }

    close(clientSocket);
    return 0;
}
