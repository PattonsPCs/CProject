#include <stdio.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>

int main(void) {
    int serverSocket_fd, clientSocket_fd = -1;
    struct sockaddr_in serverAddress, clientAddress;
    socklen_t clientLength = sizeof(clientAddress);
    int clientID = 0;
    char buffer[1024];
    char response[1024];

    serverSocket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket_fd == -1) {
        perror("Failed to create server socket");
        exit(EXIT_FAILURE);
    }
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(12345);

    if (bind(serverSocket_fd, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) {
        perror("Bind failed");
        close(serverSocket_fd);
        exit(EXIT_FAILURE);
    }

    if (listen(serverSocket_fd, 5) == -1) {
        perror("Listen failed");
        close(serverSocket_fd);
        exit(EXIT_FAILURE);
    }
    printf("Server is listening on port %d...\n", ntohs(serverAddress.sin_port));

    while (true){
        clientSocket_fd = accept(serverSocket_fd, (struct sockaddr*)&clientAddress, &clientLength);
        if (clientSocket_fd == -1) {
            perror("Accept failed");
            close(serverSocket_fd);
            continue;
        }
        clientID++;
        printf("Client %d connected!\n", clientID);

        while (true) {
            memset(buffer, 0, sizeof(buffer));
            ssize_t bytesReceived = recv(clientSocket_fd, buffer, sizeof(buffer) - 1, 0);
            if (bytesReceived < 0) {
                perror("Receive failed");
                break;
            } else if (bytesReceived == 0) {
                printf("Client disconnected.\n");
                break;
            }
            buffer[bytesReceived] = '\0';
            printf("Received message: %s\n", buffer);
            fflush(stdout);

            if (strcmp(buffer, "/quit") == 0) {
                printf("Client requested to close the connection...\n");
                break;
            }

            snprintf(response, sizeof(response), "Message Received: %s\n", buffer);
            if (send(clientSocket_fd, buffer, strlen(response), 0) == -1) {
                perror("Send failed...\n");
                break;
            }
        }
        close(clientSocket_fd);
        printf("Connection closed.\n");
    }
    close(serverSocket_fd);
    return 0;
}