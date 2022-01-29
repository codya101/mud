#include "include/fmt/format.h"
#include <string>
#include <iostream>
#include <sys/socket.h> // For socket functions
#include <netinet/in.h> // For sockaddr_in
#include <cstdlib>      // For exit() and EXIT_FAILURE
#include <unistd.h>     // For read

using namespace std;

// Create a position
// Create a map representation
// Create a player
//    with inventory
//    with equipped
//    with basic stats (strength, dexterity, intelligence)
// Create class to represent items

int main(int /*argc*/, char** /*argv*/) {

  // Create a socket (IPv4, TCP)
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd == -1) {
    std::cout << "Failed to create socket. errno: " << errno << std::endl;
    exit(EXIT_FAILURE);
  }

  // Listen to port 9999 on any address
  sockaddr_in sockaddr;
  sockaddr.sin_family = AF_INET;
  sockaddr.sin_addr.s_addr = INADDR_ANY;
  sockaddr.sin_port = htons(9999); // htons is necessary to convert a number to
                                   // network byte order

  if (bind(sockfd, (struct sockaddr*)&sockaddr, sizeof(sockaddr)) < 0) {
    std::cout << "Failed to bind to port 9999. errno: " << errno << std::endl;
    exit(EXIT_FAILURE);
  }

  // Start listening. Hold at most 10 connections in the queue
  if (listen(sockfd, 10) < 0) {
    std::cout << "Failed to listen on socket. errno: " << errno << std::endl;
    exit(EXIT_FAILURE);
  }

  // Grab a connection from the queue
  auto addrlen = sizeof(sockaddr);
  int connection = accept(sockfd, (struct sockaddr*)&sockaddr, (socklen_t*)&addrlen);
  if (connection < 0) {
    std::cout << "Failed to grab connection. errno: " << errno << std::endl;
    exit(EXIT_FAILURE);
  }

  bool keep_going = true;
  while (keep_going) {
    std::string prompt = "mud> ";
    send(connection, prompt.c_str(), prompt.size(), 0);

    // Read from the connection
    char buffer[100];
    memset(buffer, 0, 100);
    /*auto bytesRead = */ read(connection, buffer, 100);
    std::cout << "The message was: " << buffer;

    // Send a message to the connection
    std::string response = std::string("Nice command: ") + buffer + "\n";
    send(connection, response.c_str(), response.size(), 0);
  }

  // Close the connections
  close(connection);
  close(sockfd);
}