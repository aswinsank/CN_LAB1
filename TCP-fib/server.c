#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

// Function to calculate the n-th Fibonacci number
unsigned long long fibonacci(int n)
{
    if (n <= 1)
        return n;
    unsigned long long a = 0, b = 1, result = 0;
    for (int i = 2; i <= n; i++)
    {
        result = a + b;
        a = b;
        b = result;
    }
    return result;
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
        int n = atoi(buffer);
        unsigned long long result = fibonacci(n);

        snprintf(buffer, sizeof(buffer), "%llu", result);
        send(newSocket, buffer, strlen(buffer), 0);
    }

    close(newSocket);
    close(serverSocket);
    return 0;
}