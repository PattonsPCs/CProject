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
    bool loopFlag = true;
    char buffer[1024];

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
    printf("Server is listening...\n");

    while (loopFlag) {
        clientSocket_fd = accept(serverSocket_fd, (struct sockaddr*)&clientAddress, &clientLength);
        if (clientSocket_fd == -1) {
            perror("Accept failed");
            close(serverSocket_fd);
            loopFlag = false;
            continue;
        }
        clientID++;
        printf("Client %d connected!\n", clientID);

        memset(buffer, 0, sizeof(buffer));
        ssize_t bytesReceived = recv(clientSocket_fd, buffer, sizeof(buffer), 0);
        if (bytesReceived < 0) {
            perror("Receive Failed");
            close(clientSocket_fd);
            continue;
        }
        buffer[bytesReceived] = '\0';

        if (strcmp(buffer, "/quit") == 0) {
            printf("Client requested to close the connection...\n");
            close(clientSocket_fd);
            break;
        } else if (bytesReceived == 0) {
            printf("Buddy did NOT type anything...\n");
        } else {
            printf("Received message: %s\n", buffer);
        }
        strcpy(buffer, "Message Received!");
    }

    close(clientSocket_fd);
    close(serverSocket_fd);

    return 0;
}