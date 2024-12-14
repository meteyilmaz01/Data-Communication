#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h> 
#include <arpa/inet.h> 
#include <pthread.h> 
#include <time.h> 
 
#define PORT 8080 
#define MAX_CLIENTS 5 
#define BUFFER_SIZE 1024 
#define QUESTIONS_TO_ASK 4 
 
// Questions and answers 
const char *questions[] = { 
    "In which communication,the signal havingshortest wavelengt is used?\nA) Satellite communication\nB) Communication with UTP cable\nC) Communication with coaxial cable\nD) Phone communication\nE) Communication with optical fiber", 
    "Which one is wrong for packet switching?\nA) Each message is sent in packets\nB) A systemcan communicate more than one system at the same time with packet switching\nC) It is ideal for phone communication\nD) ATM based on packet switching technology\nE) None", 
    "Which property of a signal can be seen in the frequency domain?\nA) Phase\nB) Period\nC) Wavelength\nD) Spectrum\nE)All", 
    "Which definition is not true for OSI Architecture? \nA)Service ports are used in the communication between layers in the same system. \nB) For the communication between networks, Internet layer is used. \nC) Same layers in different systems communicate with protocols. \nD) Communication between upper and lower layers is done with services. \nE) Confirmed or unconfirmed communication can be done. ", 
    "Which is not true for digital signal transmission? \nA) It is done with discrete signal parts. \nB) It is used in LAN. \nC) Data rate unit is defined as bps. \nD) It gets more attenuation in transmission. \nE) It must be amplified after a certain distance.", 
    "Which one is not true for TCP/IP protocol architecture? \nA) Port address is used for identifying each computer in the network uniquely. \nB) TCP is the most reliable transport layer protocol. \nC)Isolating above layers from network technology is done with Internet layer. \nD) FTP is one of the application layer protocols. \nE) Internet is based on TCP/IP protocols. ", 
    "How does delay occur in signal transmission? \nA)Not reaching the receiver due to weakness of the signal. \nB) Signal reaches the receiver late due to interference of another signal. \nC) Because of the thermal noise, signal transmission gets slowed down. \nD) Frequencies at edges arrive to the receiver later in cable transmission. \nE) All" ,
    "Which one is done by Repeater?\nA) Providing transmission of analog signal\nB) Correcting the distortion on the digital signal\nC) Converting analog signal into digital signal\nD) Preventing delay in the transmission\nE) Reducing thermal noise "
}; 
const char answers[] = {'E', 'C', 'D', 'B', 'E', 'A', 'D', 'B'}; // Correct answers 
 
// Struct to track client data 
typedef struct { 
    int sock; 
    int score; 
} ClientData; 
 
ClientData clients[MAX_CLIENTS]; 
int client_count = 0; 
 
// Function to handle each client 
void *handle_client(void *client_socket) { 
    int sock = *(int *)client_socket; 
    free(client_socket); 
 
    char buffer[BUFFER_SIZE]; 
    int question_count = sizeof(questions) / sizeof(questions[0]); 
    int asked_questions[QUESTIONS_TO_ASK]; 
    int score = 0; 
 
    // Select random questions without repetition 
    srand(time(NULL) + sock); 
    for (int i = 0; i < QUESTIONS_TO_ASK; i++) { 
        int random_index; 
        int duplicate; 
        do { 
            duplicate = 0; 
            random_index = rand() % question_count; 
            for (int j = 0; j < i; j++) { 
                if (asked_questions[j] == random_index) { 
                    duplicate = 1; 
                    break; 
                } 
            } 
        } while (duplicate); 
        asked_questions[i] = random_index; 
    } 
 
    for (int i = 0; i < QUESTIONS_TO_ASK; i++) { 
        int question_index = asked_questions[i]; 
 
        // Send question 
        send(sock, questions[question_index], strlen(questions[question_index]), 0); 
 
        // Receive answer 
        memset(buffer, 0, BUFFER_SIZE); 
        int valread = recv(sock, buffer, BUFFER_SIZE, 0); 
        if (valread <= 0) { 
            printf("Client disconnected prematurely.\n"); 
            close(sock); 
            return NULL; 
        } 
 
        // Check answer and provide feedback 
        if (buffer[0] == answers[question_index]) { 
            score++; 
            send(sock, "Correct\n", 8, 0); 
        } else { 
            send(sock, "Wrong\n", 6, 0); 
        } 
    } 
 
    // Send final score 
    snprintf(buffer, BUFFER_SIZE, "Your final score is: %d/%d\n", score, QUESTIONS_TO_ASK); 
    send(sock, buffer, strlen(buffer), 0); 
 
    close(sock); 
    return NULL; 
} 
 
int main() { 
    int server_fd, new_socket; 
    struct sockaddr_in address; 
    int addrlen = sizeof(address); 
 
    // Create socket file descriptor 
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) { 
        perror("Socket failed"); 
        exit(EXIT_FAILURE); 
    } 
 
    // Bind to port 
    address.sin_family = AF_INET; 
    address.sin_addr.s_addr = INADDR_ANY; 
    address.sin_port = htons(PORT); 
 
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) { 
        perror("Bind failed"); 
        close(server_fd); 
        exit(EXIT_FAILURE); 
    } 
 
    // Listen for connections 
    if (listen(server_fd, MAX_CLIENTS) < 0) { 
        perror("Listen failed"); 
        close(server_fd); 
        exit(EXIT_FAILURE); 
    } 
 
    printf("Server is running on port %d\n", PORT); 
 
    // Accept clients 
    while (client_count < MAX_CLIENTS) { 
        int *client_socket = malloc(sizeof(int)); 
        if ((*client_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
            perror("Accept failed"); 
            free(client_socket); 
            continue; 
        } 
 
        printf("New client connected!\n"); 
 
        // Store client info 
        clients[client_count].sock = *client_socket; 
        clients[client_count].score = 0; 
        client_count++; 
 
        pthread_t thread_id; 
        pthread_create(&thread_id, NULL, handle_client, client_socket); 
        pthread_detach(thread_id); 
    } 
 
    close(server_fd); 
    return 0; 
}