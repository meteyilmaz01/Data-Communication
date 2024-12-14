// Client code 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h> 
#include <arpa/inet.h> 
 
#define PORT 8080 
#define BUFFER_SIZE 1024 
#define QUESTIONS_TO_ASK 4 
 
int main() { 
    int sock; 
    struct sockaddr_in server_address; 
    char buffer[BUFFER_SIZE]; 
 
    // Create socket 
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) { 
        perror("Socket creation error"); 
        return -1; 
    } 
 
    server_address.sin_family = AF_INET; 
    server_address.sin_port = htons(PORT); 
 
    // Convert IPv4 and IPv6 addresses from text to binary form 
    if (inet_pton(AF_INET, "127.0.0.1", &server_address.sin_addr) <= 0) { 
        perror("Invalid address/ Address not supported"); 
        return -1; 
    } 
 
    // Connect to the server 
    if (connect(sock, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) { 
        perror("Connection failed"); 
        return -1; 
    } 
 
    printf("Connected to the server!\n\n"); 
 
    // Receive and respond to questions 
    for (int i = 0; i < QUESTIONS_TO_ASK; i++) { 
        memset(buffer, 0, BUFFER_SIZE); 
        int valread = read(sock, buffer, BUFFER_SIZE); 
 
        if (valread <= 0) { 
            printf("Server closed the connection.\n"); 
            break; 
        } 
 
        printf("%s\n", buffer); 
 
        printf("Enter your answer: "); 
        fgets(buffer, BUFFER_SIZE, stdin); 
 
        // Send answer to server 
        send(sock, buffer, strlen(buffer), 0); 
    } 
 
    // Receive final score 
    memset(buffer, 0, BUFFER_SIZE); 
    read(sock, buffer, BUFFER_SIZE); 
    printf("%s\n", buffer); 
 
    close(sock); 
    return 0; 
}