#ifndef USER_H
#define USER_H

#include <string>

enum Permissions {
    OWNER = 1,
    ADMIN = 2,
    USER = 3

};

class User {

    User(std::string username, std:string password, Permissions perms);

    int getId();
    void setId();

    std::string getUsername();
    void setUsername();

    private:
        int id;
        std::string username;
        std::string password;
        Permissions perms;
};



#endif // USER_H