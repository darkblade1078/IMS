#include "../headers/user.h"
#include <iostream>

User::User(std::string username, std::string password, int permissionLevel) {
    this->username = username;
    this->password = password;
    this->permissionLevel = permissionLevel;
}

std::string User::getUsername() {
    return username;
}

std::string User::getPassword() {
    return password;
}

int User::getPermissionLevel() {
    return permissionLevel;
}