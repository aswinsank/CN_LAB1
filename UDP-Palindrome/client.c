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
    socklen_t addr_size = sizeof(serverAddr);

    clientSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (clientSocket < 0)
    {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    printf("Enter a string to check if it is a palindrome: ");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';

    sendto(clientSocket, buffer, strlen(buffer), 0, (struct sockaddr *)&serverAddr, addr_size);

    memset(buffer, 0, sizeof(buffer));
    recvfrom(clientSocket, buffer, sizeof(buffer), 0, (struct sockaddr *)&serverAddr, &addr_size);
    printf("Server response: %s\n", buffer);

    close(clientSocket);
    return 0;
}
