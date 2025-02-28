#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

int main(void){
  int socket_fd;
  struct sockaddr_in serverAddress;
  char buffer[1024] = "Hello Server!";

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

  send(socket_fd, buffer, strlen(buffer), 0);
  printf("Message sent: %s\n", buffer);

  memset(buffer, 0, sizeof(buffer));
  recv(socket_fd, buffer, sizeof(buffer), 0);
  printf("Server resposne: %s\n", buffer);

  close(socket_fd);
  return 0;

}