#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <math.h>

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
        exit(1);
    }

    while (1) {
        memset(buffer, 0, BUFFER_SIZE);

        // Receive the number from the client
        int valread = recv(new_sockfd, buffer, BUFFER_SIZE, 0);
        if (valread == -1) {
            perror("recv");
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
        send(new_sockfd, result, strlen(result), 0);
    }

    // Close sockets
    close(new_sockfd);
    close(sockfd);
    return 0;
}
