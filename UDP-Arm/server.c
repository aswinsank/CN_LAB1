#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 8080
#define BUFFER_SIZE 1024

// Function to check if a number is an Armstrong number
int is_armstrong(int num) {
    int original_num = num;
    int sum = 0;
    int digits = 0;
    // Count the number of digits in the number
    while (num != 0) {
        num /= 10;
        digits++;
    }

    num = original_num;
    // Check if it's an Armstrong number
    while (num != 0) {
        int digit = num % 10;
        sum += pow(digit, digits);
        num /= 10;
    }

    return sum == original_num;
}

int main() {
    int sockfd;
    int addrlen = sizeof(struct sockaddr_in);
    struct sockaddr_in my_addr, client_addr;
    char buffer[BUFFER_SIZE] = {0};

    // Create socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
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

    while (1) {
        memset(buffer, 0, BUFFER_SIZE);

        // Receive the number from the client
        int valread = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&client_addr, &addrlen);
        if (valread == -1) {
            perror("recvfrom");
            break;
        }

        int num = atoi(buffer);  // Convert the received string to an integer

        // Check if the number is an Armstrong number
        char result[BUFFER_SIZE];
        if (is_armstrong(num)) {
            snprintf(result, sizeof(result), "%d is an Armstrong number.", num);
        } else {
            snprintf(result, sizeof(result), "%d is not an Armstrong number.", num);
        }

        // Send the result back to the client
        sendto(sockfd, result, strlen(result), 0, (struct sockaddr *)&client_addr, addrlen);
    }

    close(sockfd);
    return 0;
}
