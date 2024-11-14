#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

unsigned long long factorial(int num)
{
    if (num < 0)
        return 0; // Factorial not defined for negative numbers
    unsigned long long result = 1;
    for (int i = 1; i <= num; i++)
    {
        result *= i;
    }
    return result;
}

int main()
{
    int serverSocket;
    struct sockaddr_in serverAddr, clientAddr;
    char buffer[1024];
    socklen_t clientAddrLen = sizeof(clientAddr);

    serverSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (serverSocket < 0)
    {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
    {
        perror("Binding failed");
        close(serverSocket);
        exit(EXIT_FAILURE);
    }

    printf("Server is listening on port 8080...\n");

    while (1)
    {
        int recvLen = recvfrom(serverSocket, buffer, sizeof(buffer), 0, (struct sockaddr *)&clientAddr, &clientAddrLen);
        if (recvLen < 0)
        {
            perror("Receive failed");
            continue;
        }

        buffer[recvLen] = '\0';
        int number = atoi(buffer);
        unsigned long long result = factorial(number);

        snprintf(buffer, sizeof(buffer), "%llu", result);
        sendto(serverSocket, buffer, strlen(buffer), 0, (struct sockaddr *)&clientAddr, clientAddrLen);
    }

    close(serverSocket);
    return 0;
}
