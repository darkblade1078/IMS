#include "../headers/protocols.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <iostream>
#include <sys/wait.h>

int main()
{
    //creating socket
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    //specifying the address of the server
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    //bind the socket to our specified IP and port
    bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress));

    //listen to the assigned socket
    listen(serverSocket, 5);

    std::cout << "Server is listening on port 8080..." << std::endl;

    //infinite loop to keep the server running
    while(true) {

        //accepting connection request
        int clientSocket = accept(serverSocket, nullptr, nullptr);
        
        //check if connection was successful
        if(clientSocket < 0) {
            std::cerr << "Error accepting connection" << std::endl;
            continue;
        }

        //create a child process to handle the client
        pid_t pid = fork();

        //child process
        if(pid == 0) {

            //child doesn't need the server socket
            close(serverSocket);

            //handle the client that is trying to connect
            handleClient(clientSocket);

            //exit child
            exit(0);
        }
        //parent process
        else {
            
            //parent doesn't need the client socket
            close(clientSocket);

            //clean up zombie processes
            while(waitpid(-1, NULL, WNOHANG) > 0);
        }
    }

    //close the client
    close(serverSocket);

    return 0;
}