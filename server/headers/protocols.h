#ifndef PROTOCOLS_H
#define PROTOCOLS_H

#include <string>
#include <unordered_map>

struct User {
    std::string username;
    std::string password;
};

//temp definitions
typedef std::unordered_map<std::string, User> UserMap;

void handleClient(int client_socket); //handle new clients
void handleLogin(std::string command); //handle login requests

#endif // PROTOCOLS_H