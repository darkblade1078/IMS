#ifndef DATABASE_H
#define DATABASE_H

#include <sqlite3.h>
#include <optional>
#include "./headers/user.h"

class Database {
    Database();
    ~Database();

    std::optional<User> findUser(std::string username, std::string password);
    bool checkUser();
    bool createUser(std::string username, std::string password, int perms);
    bool deleteUser();
}


#endif // DATABASE_H