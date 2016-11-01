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

//returns size of arrays
int read_file (char usernames[][], char passwords[][]) {
  int i = 0;
  FILE* fp = fopen("users.txt", "r");

  while (!feof(fp)) {
    fscanf(fp, "(%s, %s)", usernames[i], passwords[i]);
    i++;
  }

  return i; //Number of users ()
}

int process_client_request (char client_request[]) {


}
int main () {

  //Output to client
  char server_message[256];

  //Arrays of usernames & passwords
  char usernames[256][256];
  char passwords[256][256];

  //If no usernames or passwords are read in, an error occured
  int number_of_users = read_file(usernames, passwords);
  if (number_of_users == 0) {
    printf("Error Reading File\n");
    return 1;
  }
  number_of_users++; //Account for off by one

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

  //Get request from client
  char client_request[256];
  recv(client_socket, &client_request, sizeof(client_request), 0);

  char action[32];
  char user_id[32];
  char password[8];
  char message[256];

  sscanf(client_request, )
  switch (client_request) {
    case "login":
      login
  }

  //send message
  send(client_socket, server_message, sizeof(server_message), 0);

  close(server_socket);

  return 0;
}
