#include <stdio.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>

int main(void){
    int serverSocket_fd, clientSocket_fd;
    struct sockaddr_in serverAddress, clientAddress;
    socklen_t clientLength = sizeof(clientAddress);
    int clientID = 0;

    serverSocket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket_fd == -1) {
        perror("Failed to create server socket");
        exit(EXIT_FAILURE);
    }
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(12345);

    if (bind(serverSocket_fd, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) {
        perror("Bind failed.");
        close(serverSocket_fd);
        exit(EXIT_FAILURE);
    }


    if (listen(serverSocket_fd, 5) == -1) {
        perror("Listen failed");
        close(serverSocket_fd);
        exit(EXIT_FAILURE);
    }
    printf("Server is listening...");


        clientSocket_fd = accept(serverSocket_fd, (struct sockaddr*)&clientAddress, &clientLength);
        if (clientSocket_fd == -1) {
            perror("Accept failed");
            close(serverSocket_fd);
            exit(EXIT_FAILURE);
        }
        clientID++;
        printf("Client %d connected!", clientID);


    close(clientSocket_fd);
    close(serverSocket_fd);

    return 0;
}