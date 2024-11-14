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
    socklen_t addrLen = sizeof(serverAddr);

    clientSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (clientSocket < 0)
    {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    printf("Enter an integer n to calculate the n-th Fibonacci number: ");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';

    sendto(clientSocket, buffer, strlen(buffer), 0, (struct sockaddr *)&serverAddr, addrLen);

    int recvLen = recvfrom(clientSocket, buffer, sizeof(buffer), 0, (struct sockaddr *)&serverAddr, &addrLen);
    if (recvLen > 0)
    {
        buffer[recvLen] = '\0';
        printf("Fibonacci result: %s\n", buffer);
    }

    close(clientSocket);
    return 0;
}
