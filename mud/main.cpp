#include "include/fmt/format.h"
#include <string>
#include <iostream>
#include <sys/socket.h> // For socket functions
#include <netinet/in.h> // For sockaddr_in
#include <cstdlib>      // For exit() and EXIT_FAILURE
#include <unistd.h>     // For read
#include <stdlib.h>
#include <algorithm>
#include <vector>

using namespace std;

// Create a position
// Create a map representation
// Create a player
//    with inventory
//    with equipped
//    with basic stats (strength, dexterity, intelligence)
// Create class to represent items

int create_socket() {
  // Create a socket (IPv4, TCP)
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd == -1) {
    std::cout << "Failed to create socket. errno: " << errno << std::endl;
    exit(EXIT_FAILURE);
  }

  return sockfd;
}

int get_port(int argc, char** argv) {
  int port = 9999;
  if (argc > 1) {
    port = atoi(argv[1]);
  }
  std::cout << "The port is " << port << std::endl;
  return port;
}

void init_sockaddr(sockaddr_in* p_sa, int port) {
  p_sa->sin_family = AF_INET;
  p_sa->sin_addr.s_addr = INADDR_ANY;
  p_sa->sin_port = htons(port); // htons is necessary to convert a number to network byte order
}

int main(int argc, char** argv) {
  int port = get_port(argc, argv);
  int sockfd = create_socket();
  int maxfd = sockfd;

  sockaddr_in sockaddr;
  init_sockaddr(&sockaddr, port);

  if (bind(sockfd, (struct sockaddr*)&sockaddr, sizeof(sockaddr)) < 0) {
    std::cout << "Failed to bind to port 9999. errno: " << errno << std::endl;
    exit(EXIT_FAILURE);
  }

  // Start listening. Hold at most 10 connections in the queue
  if (listen(sockfd, 5) < 0) {
    std::cout << "Failed to listen on socket. errno: " << errno << std::endl;
    exit(EXIT_FAILURE);
  }

  int connection = -1;
  std::vector<int> connections;
  fd_set rset, allset;
  int nready = 0;
  FD_ZERO(&allset);
  FD_SET(sockfd, &allset);
  bool keep_going = true;

  while (keep_going) {
    rset = allset; /* structure assignment */

    cout << "before select..." << endl;
    nready = select(maxfd + 1, &rset, NULL, NULL, NULL);
    cout << "after select..." << endl;

    if (FD_ISSET(sockfd, &rset)) {
      auto addrlen = sizeof(sockaddr);
      connection = accept(sockfd, (struct sockaddr*)&sockaddr, (socklen_t*)&addrlen); // blocks
      if (connection < 0) {
        std::cout << "Failed to grab connection. errno: " << errno << std::endl;
        exit(EXIT_FAILURE);
      }

      std::cout << "connection is " << connection << std::endl;
      connections.push_back(connection);
      FD_SET(connection, &allset);

      if (connection > maxfd)
        maxfd = connection;

      std::string prompt = "mud> ";
      send(connection, prompt.c_str(), prompt.size(), 0);

      if (--nready <= 0)
        continue; /* no more readable descriptors */
    }

    for (auto c : connections) {
      if (FD_ISSET(c, &rset)) {
        uint8_t buffer[100];
        memset(buffer, 0, 100);
        ssize_t n = read(c, buffer, 100);

        if (n == 0 || n == -1 || (n == 1 && buffer[0] == 4) || buffer[0] == 0xff) {
          /* connection closed by client */
          close(c);
          FD_CLR(c, &allset);
          auto it = find(connections.begin(), connections.end(), c);
          connections.erase(it);
        } else {
          std::cout << "The message was: " << buffer << std::endl;

          // Send a message to the connection
          std::string response = std::string("Nice command: ") + (char*)buffer + "\n";
          send(connection, response.c_str(), response.size(), 0);

          std::string prompt = "mud> ";
          send(c, prompt.c_str(), prompt.size(), 0);
        }

        if (--nready <= 0)
          break; /* no more readable descriptors */
      }
    }
  }

  // Close the connections
  close(connection);
  close(sockfd);
}