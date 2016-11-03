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
  return i; //Number of users ()
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
      // if (username == usernames[i] && password == passwords[i]) {
      //   return 1; //Valid user
      // }

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

  //Get response from client
  int client_socket;

  printf( "Waiting for a client to connect...\n" );

  while (1) {
    client_socket = accept(server_socket, NULL, NULL); //Last two parameters left as NULL becuase it's all local

    printf( "Client Connected.\n");

  //Get request from client
  char client_request[MAX_LINE];
  int length = recv(client_socket, client_request, MAX_LINE, 0);
  client_request[length] = 0; //End string with a NULL terminator

  int login_result = 0;
  //action = strtok(client_request," ");//Parse Action
  strcpy(action, strtok(client_request," "));
  //Forward action
  if (action == "login") {
    user_id = strtok(client_request," "); //Get user ID from request
    password = strtok(client_request," ");
    login_result = login(user_id, password);
    if (login_result == 1) {
      char server_message[MAX_LINE] = "Login";
    } else {
      char server_message[MAX_LINE] = "Invalid User";
    }
    break;
  } else if (action == "newuser") {
    char user_id[32] = strtok(client_request," "); //Get user ID from request
    char password[8] = strtok(client_request," ");

    if (create_new_user(user_id, password) == 1) {
      char server_message[MAX_LINE] = "User Created";
    } else {
      char server_message[MAX_LINE] = "User Could not be Created";
    }
  } else if (action == "send") {
    if (login_result != 1) {
      char server_message[MAX_LINE] = "Not logged in";
      break;
    }
    char message[MAX_LINE] = strtok(client_request, " ");
    char server_message[MAX_LINE] = user_id;
    strcat(server_message, " ");
    strcat(server_message, message);
  } else if (action == "logout") {
    login_result = 0;
    server_message = "Logout"
  } else {
    printf("Invalid Command\n");
  }

  //send message
  send(client_socket, server_message, sizeof(server_message), 0);
  close(client_socket);

} //End While

  close(server_socket);
  return 0;
}
