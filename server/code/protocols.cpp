#include "../headers/protocols.h"
#include <sys/socket.h>
#include <iostream>
#include <unistd.h>
#include <string.h>
#include <sstream>

void handleClient(int clientSocket) {
    char buffer[1024]; //buffer to store the message from the client

    while (true) {

        //set memory blocks to the size of our buffer
        memset(buffer, 0, sizeof(buffer));

        //receive message from client
        int bytesReceived = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);

        //check if the client disconnected
        if(bytesReceived < 0)
            break;

        //create the command variable from the buffer
        std::string command(buffer);

        if (command.rfind("LOGIN", 0) == 0)
            handleLogin(command);
    }

    close(clientSocket);
}

void handleLogin(std::string command) {

    //test data until we have a database implemented
    UserMap users = {
        {"darkblade", {"darkblade", "12345"}},
    };

    //splitting the command, username, and password into separate strings
    std::istringstream iss(command);
    std::string login_cmd, username, password;
    iss >> login_cmd >> username >> password;

    //check if the user exists
    if (users.find(username) == users.end()) {
        std::cout << "User not found" << std::endl;
        return;
    }

    //check if the password is correct
    if (users[username].password == password) {
        std::cout << "User logged in" << std::endl;
    } else {
        std::cout << "Incorrect password" << std::endl;
    }
}