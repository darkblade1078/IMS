#ifndef USER_H
#define USER_H

#include <string>

class User {
    private:
        std::string username;
        std::string password;
        int permissionLevel; //0 = owner, 1 = admin, 2 = user

    public:
        User(std::string username, std::string password, int permissionLevel);
        std::string getUsername();
        std::string getPassword();
        int getPermissionLevel();
};

#endif 