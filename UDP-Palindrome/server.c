#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

int isPalindrome(char *str)
{
    int len = strlen(str);
    for (int i = 0; i < len / 2; i++)
    {
        if (str[i] != str[len - i - 1])
        {
            return 0;
        }
    }
    return 1;
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
        memset(buffer, 0, sizeof(buffer));
        recvfrom(serverSocket, buffer, sizeof(buffer), 0, (struct sockaddr *)&clientAddr, &clientAddrLen);

        char *response = isPalindrome(buffer) ? "YES" : "NO";

        sendto(serverSocket, response, strlen(response), 0, (struct sockaddr *)&clientAddr, clientAddrLen);
    }

    close(serverSocket);
    return 0;
}
