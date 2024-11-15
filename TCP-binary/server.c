#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 8080
#define BUFFER_SIZE 1024

// Function to convert a decimal number to binary
void decimal_to_binary(int num, char *binary) {
    int index = 0;
    if (num == 0) {
        binary[index++] = '0';
    } else {
        while (num > 0) {
            binary[index++] = (num % 2) + '0';
            num = num / 2;
        }
    }
    binary[index] = '\0';

    // Reverse the string since binary is calculated from least significant bit
    int start = 0;
    int end = index - 1;
    while (start < end) {
        char temp = binary[start];
        binary[start] = binary[end];
        binary[end] = temp;
        start++;
        end--;
    }
}

int main() {
    int sockfd, new_sockfd;
    struct sockaddr_in my_addr, client_addr;
    int addrlen = sizeof(struct sockaddr_in);
    char buffer[BUFFER_SIZE] = {0};

    // Create TCP socket
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }

    // Setup server address
    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(PORT);
    my_addr.sin_addr.s_addr = INADDR_ANY;

    // Bind the socket
    if (bind(sockfd, (struct sockaddr *)&my_addr, addrlen) == -1) {
        perror("bind");
        exit(1);
    }

    // Listen for incoming connections
    if (listen(sockfd, 3) == -1) {
        perror("listen");
        exit(1);
    }

    printf("\nServer listening on port %d...\n", PORT);

    // Accept client connection
    if ((new_sockfd = accept(sockfd, (struct sockaddr *)&client_addr, (socklen_t *)&addrlen)) == -1) {
        perror("accept");
    
