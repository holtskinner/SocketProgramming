/*
  Lab 3 (Part 1) - Socket Programming
  Holt Skinner
  11-4-2016
  Chat Room with one client Using Socket API in C

  Server Side
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/in.h>

#define PORT          14311
#define MAX_PENDING   5
#define MAX_LINE      256

// Return 1 if valid user, 0 otherwise
int login (char* username, char* password) {
    //Arrays of usernames & passwords
    char usernames[256][256];
    char passwords[256][256];

    FILE* fp = fopen("users.txt", "r");

    int number_of_users = 0;
    int i = 0;

    for (i = 0; !feof(fp); i++) {
      fscanf(fp, "%s %s\n", usernames[i], passwords[i]);
    }

    number_of_users = i;

    fclose(fp);

    if (number_of_users == 0) {
      printf("Error Reading File\n");
      return 1;
    }

    int tempUser = 1, tempPass = 1;
    for (i = 0; i < number_of_users; i++) {

      tempUser = strcmp(username, usernames[i]);
      tempPass = strcmp(password, passwords[i]);

      if (tempUser == 0 && tempPass == 0) {
        return 1;
      }
    }
    return 0; //Invalid user
}

//Return 1 if user created, 0 otherwise
int create_new_user (char* username, char* password) {

    if (strlen(username) > 32 || strlen(username) <= 0) {
      return 0;
    }
    if (strlen(password) > 8 || strlen(password) < 4) {
      return 0;
    }

    //check if user already exists
    if (login(username, password) == 1) {
      return 0;
    }

    FILE* fp = fopen("users.txt", "a");

    fprintf(fp, "%s %s\n", username, password); //Add user to file
    fclose(fp);

    return 1; //User created
}

int main () {

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
  listen(server_socket, MAX_PENDING);

  printf( "Waiting for a client to connect...\n" );

  int client_socket = accept(server_socket, NULL, NULL); //Last two parameters left as NULL becuase it's all local
  printf( "Client Connected.\n");

  //Get response from client
  char* server_message = malloc(sizeof(char) * MAX_LINE); //Sent from server to client
  char client_request[MAX_LINE]; //From client to server
  char* user_id = NULL;
  char* password = NULL;
  char* message = NULL;
  char* action = NULL;
  int login_result = 0;

  while (1) {

    //Get request from client, leave if client quits
    if (recv(client_socket, client_request, sizeof(client_request), 0) == 0) {
      break;
    }

    action = strtok(client_request, "\n"); //Parse Single Word Responses
    action = strtok(client_request, " "); //Parse Multiple Word Responses

    if (strcmp(action, "exit") == 0) { //end gracefully
      break;
    } else if (strcmp(action, "login") == 0) { //Login
          user_id = strtok(NULL, " "); //Get user ID from request
          password = strtok(NULL, "\n");

          login_result = login(user_id, password);

          if (login_result == 1) {
            server_message = "Success\0";
          } else {
            server_message = "Not User\0";
          }

    } else if (strcmp(action, "logout") == 0) { //Logout
          if(login_result == 1) { //If Logged in
            login_result = 0;
            server_message = "Success\0";
          } else { //If not logged in
            server_message = "Invalid\0";
          }
    } else if (strcmp(action, "newuser") == 0) {
          user_id = strtok(NULL, " "); //Get user ID from request
          password = strtok(NULL, "\n");

          if (create_new_user(user_id, password) == 1) {
            server_message = "Success\0";
          } else {
            server_message = "Error\0";
          }
    } else if (strcmp(action, "send") == 0) {

          if (login_result != 1) {
            server_message = "Fail\0";
          } else {
            message = strtok(NULL, "\n");
            strcat(server_message, user_id);
            strcat(server_message, ": ");
            strcat(server_message, message);
          }
    }
    else { //Other
          server_message = "Invalid\0";
    }
    //send message
    send(client_socket, server_message, sizeof(server_message), 0);

} //End While

  printf("%s\n", "Client Disconnected");
  //free(server_message);

  shutdown(client_socket, 2);
  shutdown(server_socket, 2);
  return 0;
}
