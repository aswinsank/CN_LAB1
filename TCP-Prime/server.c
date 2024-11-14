#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

int isPrime(int num)
{
    if (num <= 1)
        return 0;
    if (num == 2 || num == 3)
        return 1;
    if (num % 2 == 0 || num % 3 == 0)
        return 0;
    for (int i = 5; i * i <= num; i += 6)
    {
        if (num % i == 0 || num % (i + 2) == 0)
            return 0;
    }
    return 1;
}

int main()
{
    int serverSocket, newSocket;
    struct sockaddr_in serverAddr, clientAddr;
    char buffer[1024];
    socklen_t addrSize;

    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
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

    if (listen(serverSocket, 5) < 0)
    {
        perror("Listening failed");
        close(serverSocket);
        exit(EXIT_FAILURE);
    }

    printf("Server is listening on port 8080...\n");

    addrSize = sizeof(clientAddr);
    newSocket = accept(serverSocket, (struct sockaddr *)&clientAddr, &addrSize);
    if (newSocket < 0)
    {
        perror("Accept failed");
        close(serverSocket);
        exit(EXIT_FAILURE);
    }

    int recvLen = recv(newSocket, buffer, sizeof(buffer), 0);
    if (recvLen < 0)
    {
        perror("Receive failed");
    }
    else
    {
        buffer[recvLen] = '\0';
        int number = atoi(buffer);
        const char *response = isPrime(number) ? "YES" : "NO";
        send(newSocket, response, strlen(response), 0);
    }

    close(newSocket);
    close(serverSocket);
    return 0;
}
