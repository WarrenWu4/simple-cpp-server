#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h> // import library for sockets (doesn't work for windows -_-)
#include <netinet/in.h>

int main()
{
  const int PORT = 3000;
  int activeConnections = 0;

  // create the socket
  int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
  if (serverSocket == -1)
  {
    std::cerr << "Socket creation failed.\n";
    return 1;
  }

  // bind the socket to an address and port
  sockaddr_in serverAddr;
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_addr.s_addr = INADDR_ANY;
  serverAddr.sin_port = htons(PORT);

  if (bind(serverSocket, (sockaddr *)&serverAddr, sizeof(serverAddr)) == -1)
  {
    std::cerr << "Binding failed.\n";
    return 1;
  }

  // listen to incoming connections
  if (listen(serverSocket, 10) == -1)
  {
    std::cerr << "Listening failed.\n";
    return 1;
  }

  std::cout << "Server running on port: " << PORT << "\n";

  while (true)
  {
    // accept incoming connections
    sockaddr_in clientAddr;
    socklen_t clientAddrSize = sizeof(clientAddr);
    int clientSocket = accept(serverSocket, (sockaddr *)&clientAddr, &clientAddrSize);
    if (clientSocket == -1)
    {
      std::cerr << "Accepting Connections Failed\n";
      continue;
    }

    // track number of active connections
    activeConnections++;
    std::cout << activeConnections << " number of active connections\n";

    // send http response if connection is successful
    const char *response = "HTTP/1.1 200 OK\nContent-Type: text/html\n\n<html><body><h1>bruh moment</h1></body></html>";
    send(clientSocket, response, strlen(response), 0);

    // close client connection & update number of active connections
    close(clientSocket);
    activeConnections--;
    std::cout << activeConnections << " number of active connections\n";
  }

  close(serverSocket);

  return 0;
}
