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

//returns size of arrays
int read_file (char usernames[MAX_LINE][MAX_LINE], char passwords[MAX_LINE][MAX_LINE]) {
  int i = 0;
  FILE* fp = fopen("users.txt", "r+");

  while (!feof(fp)) {
    fscanf(fp, "(%s, %s)", usernames[i], passwords[i]);
    i++;
  }
  fclose(fp);
  return i; //Number of users
}

// Return 1 if valid user, 0 otherwise
int login (char username[], char password[]) {
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

    int i = 0;
    int tempUser = 0, tempPass = 0;
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
int create_new_user (char username[], char password[]) {
    if (strlen(username) >= 32 || strlen(username) <= 0) {
      return 0;
    }
    if (strlen(password) > 8 || strlen(password) < 4) {
      return 0;
    }
    //Arrays of usernames & passwords
    char usernames[256][256];
    char passwords[256][256];

    //If no usernames or passwords are read in, an error occured
    int number_of_users = read_file(usernames, passwords);
    if (number_of_users == 0) {
      printf("Error Reading File\n");
      return 0;
    }
    number_of_users++; //Account for off by one
    int i = 0;
    int tempUser = 0;
    //Search existing users for username
    for (i = 0; i < number_of_users; i++) {

      tempUser = strcmp(username, usernames[i]);

      if (tempUser == 0) {
        return 0; //User already exists
      }
    }

    FILE* fp = fopen("users.txt", "w");

    while (!feof(fp)) {
      fscanf(fp, "(%s, %s)", usernames[i], passwords[i]);
      i++;
    }

    fprintf(fp, "(%s, %s)", username, password); //Add user to file
    fclose(fp);
    return 1;//User created
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
  char server_message[MAX_LINE]; //Sent from server to client
  char client_request[MAX_LINE]; //From client to server
  char *user_id;
  char *password;
  char *message;
  char *action;

  while (1) {

    //Get request from client
    recv(client_socket, client_request, MAX_LINE, 0);

    int login_result = 0;

    action = strtok(client_request, " "); //Parse Action

    //strcpy(server_message, client_request);
    //printf("%s\n", server_message);

    //Forward action
    // int result = strcmp(action, "login");
    // printf("%d\n", result);

    if (strcmp(action, "login") == 0) {

      // user_id = strtok(client_request," ");
      // password = strtok(client_request," ");
      printf("%s\n", "login");

      user_id = strtok(NULL, " "); //Get user ID from request
      password = strtok(NULL, " ");

      printf("%s\n", user_id);
      printf("%s\n", password);

      login_result = login(user_id, password);
      if (login_result == 1) {
        strcpy(server_message, "Login");
        // server_message[MAX_LINE] = "Login";
      } else {
        strcpy(server_message, "Invalid User");
        //char server_message[MAX_LINE] = "Invalid User";
      }
    }
    // } else if (strcmp(action, "newuser")) {
    //     strcpy(user_id, strtok(client_request," ")); //Get user ID from request
    //     strcpy(password, strtok(client_request," ")); //Get password from request
    //
    //     if (create_new_user(user_id, password) == 1) {
    //       strcpy(server_message, "User Created");
    //     } else {
    //       strcpy(server_message, "User could not be Created");
    //     }
    //
    // } else if (strcmp(action, "send")) {
    //
    //   if (login_result != 1) {
    //     strcpy(server_message, "Not Logged In");
    //     break;
    //   }
    //
    //   strcpy(message, strtok(client_request, " ")); //get message to be sent
    //   strcpy(server_message, user_id);
    //
    //   strcat(server_message, " ");
    //   strcat(server_message, message);
    //
    // } else if (strcmp(action, "logout")) {
    //
    //   login_result = 0;
    //   strcpy(server_message, "Logout");
    //
    // } else {
    //
    //   strcpy(server_message, "Invalid");
    //
    // } //End routing

    //send message
    send (client_socket, server_message, sizeof(server_message), 0);

} //End While
  close (client_socket);
  close (server_socket);
  return 0;
}
