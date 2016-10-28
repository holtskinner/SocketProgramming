/*
  Lab 3 (Part 1) - Socket Programming
*/
#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/in.h>

int main() {

  //create socket
  int networkSocket = socket(AF_INET, SOCK_STREAM, 0);

  
  return 0;
}
