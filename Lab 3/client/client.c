/*
  Lab 3 (Part 1) - Socket Programming
  Holt Skinner
  11-4-2016
  Chat Room with one client Using Socket API in C

  Client Side
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#define PORT      14311
#define MAX_LINE  256

int main () {

  //create socket
  int network_socket = socket(AF_INET, SOCK_STREAM, 0);

  //Address for socket
  struct sockaddr_in server_address;
  server_address.sin_family = AF_INET;

  //Converts port number to struct format
  server_address.sin_port = htons(PORT);
  //IP Address
  server_address.sin_addr.s_addr = INADDR_ANY; //0.0.0.0

  //Returns 0 if ok, -1 if something goes wrong
  int connection_status = connect(network_socket, (struct sockaddr *) &server_address, sizeof(server_address));

  if (connection_status == -1) {
    printf("Connection Error\n");
    return 1;
  }

  printf("My chat room client. Version One.\n");

  char input[256];
  char* server_response = malloc(sizeof(char) * MAX_LINE);

  while (1) {

    fgets(input, 256, stdin);

    //send message to server
    send(network_socket, input, sizeof(input), 0);

    //Recieve data from server, end loop when server closes
    if (recv(network_socket, server_response, sizeof(server_response), 0) == 0) {
      break;
    }

    //Print out recieved data
    printf("%s\n", server_response);

  }
  //Close Socket
  printf("Bye!\n");
  free(server_response);
  shutdown(network_socket, 2);
  return 0;
}
