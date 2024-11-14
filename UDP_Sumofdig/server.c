#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

int calculateSumOfDigits(const char *number)
{
    int sum = 0;
    while (*number)
    {
        if (*number >= '0' && *number <= '9')
        {
            sum += *number - '0';
        }
        number++;
    }
    return sum;
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

    while (1)
    {
        int recvLen = recvfrom(serverSocket, buffer, sizeof(buffer), 0, (struct sockaddr *)&clientAddr, &clientAddrLen);
        if (recvLen < 0)
        {
            perror("Receive failed");
            continue;
        }

        buffer[recvLen] = '\0';
        int sum = calculateSumOfDigits(buffer);
        snprintf(buffer, sizeof(buffer), "%d", sum);

        sendto(serverSocket, buffer, strlen(buffer), 0, (struct sockaddr *)&clientAddr, clientAddrLen);
    }

    close(serverSocket);
    return 0;
}
