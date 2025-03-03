#include <stdio.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>

int main(void) {
    int socket_fd;
    struct sockaddr_in serverAddress;
    char buffer[1024];
    bool loopFlag = true;

    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd == -1) {
        perror("Socket creation failed.");
        exit(EXIT_FAILURE);
    }

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(12345); // Ensure this matches the server port
    serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(socket_fd, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) {
        perror("Connection failed");
        close(socket_fd);
        exit(EXIT_FAILURE);
    }
    printf("Connected to server!\n");

    while (loopFlag) {
        printf("Enter a message to send to the server:\n");
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            perror("Input failed");
            break;
        }
        buffer[strcspn(buffer, "\n")] = '\0'; // Remove the newline character

        if (strcmp(buffer, "/quit") == 0) {
            printf("Closing connection...\n");
            loopFlag = false;
        }

        ssize_t totalSent = 0;
        ssize_t length = strlen(buffer);
        while (totalSent < length) {
            ssize_t bytesSent = send(socket_fd, buffer + totalSent, length - totalSent, 0);
            if (bytesSent < 0) {
                perror("Send failed");
                break;
            }
            totalSent += bytesSent;
        }
        memset(buffer, 0, sizeof(buffer));
        ssize_t bytesReceived = recv(socket_fd, buffer, sizeof(buffer) - 1, 0);
        if (bytesReceived < 0) {
            perror("Receive failed");
        } else if (bytesReceived == 0) {
            printf("Server disconnected.\n");
            break;
        }
        buffer[bytesReceived] = '\0';
        printf("Server response: %s\n", buffer);
    }

    close(socket_fd);
    return 0;
}