/*
  Lab 3 (Part 1) - Socket Programming
  Holt Skinner
  11-4-2016
  Chat Room with one client Using Socket API in C

  Server Side
*/
#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/in.h>

#define PORT 14311

int main() {

  char server_message[256] = "Hello World!";

  //create server socket
  int server_socket = socket(AF_INET, SOCK_STREAM, 0);

  //Address for socket
  struct sockaddr_in server_address;
  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(PORT);
  server_address.sin_addr.s_addr = INADDR_ANY; //local

  //Bind to recieve response
  bind(server_socket, (struct sockaddr *) &server_address, sizeof(server_address));

  //Wait for response
  listen(server_socket, 1); //Allow 1 user at a time

  //Get response from client
  int client_socket = accept(server_socket, NULL, NULL);//Last two parameters left as NULL becuase it's all local
  //send message
  send(client_socket, server_message, sizeof(server_message), 0);

  close(server_socket);
  
  return 0;
}
