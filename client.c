#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>

int main(void){
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
  serverAddress.sin_port = htons(12345);
  serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");

  if (connect(socket_fd, (struct sockaddr*)&serverAddress,  sizeof(serverAddress)) == -1) {
    perror("Connection failed");
    close(socket_fd);
    exit(EXIT_FAILURE);
  }
  printf("Connected to server!\n");

  while (loopFlag) {
    printf("Enter a message to send to the server:\n");
    scanf("%s", buffer);
    if (strcmp(buffer, NULL) == 0) {
      printf("Come on, you gotta send something bro...\n");
    } else if (strcmp(buffer, "/quit") == 0) {
      printf("Closing connection...");
      loopFlag = false;
    } else {
      send(socket_fd, buffer, sizeof(buffer), 0);
    }

    memset(buffer, 0, sizeof(buffer));
    recv(socket_fd, buffer, sizeof(buffer), 0);
    printf("Server response: %s\n", buffer);
  }

  close(socket_fd);
  return 0;

}