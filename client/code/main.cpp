#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

int main()
{
    //creating the socket
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);

    //specifying address
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    //sending connection request
    connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress));

    //sending data (this is a c string btw)
    const char* message = "LOGIN darkblade 12345";
    send(clientSocket, message, strlen(message), 0);

    //closing socket
    close(clientSocket);

    return 0;
}