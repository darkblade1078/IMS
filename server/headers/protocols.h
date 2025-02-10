#ifndef PROTOCOLS_H
#define PROTOCOLS_H

#include "./database.h"
#include "./user.h"
#include <string>
#include <unordered_map>

void handleClient(int client_socket); //handle new clients
void handleLogin(std::string command); //handle login requests

#endif // PROTOCOLS_H