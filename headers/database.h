#ifndef DATABASE_H
#define DATABASE_H

#include "user.h"
#include "inventory.h"
#include <sqlite3.h>
#include <string>
#include <optional>

class Database {

    public:
        Database(); //constructor
        ~Database(); //destructor

        /*
            User Methods
        */
        int getUserCount(); //returns the number of users in the database
        bool createFirstUser(std::string username, std::string password); //creates the first user
        bool createUser(User admin, std::string username, std::string password, int permissionLevel); //creates a new user
        bool deleteUser(User admin, std::string username); //deletes a user
        bool changePermissions(User admin, std::string username, int permissionLevel); //changes the permissions of a user
        bool updatePassword(User admin, std::string username, std::string newPassword); //updates the password of a user
        std::optional<User> login(std::string username, std::string password); //allows the user to login into their account on the terminal

        /*
            Inventory Methods
        */
        bool createItem(User admin, std::string itemName, int quantity);

        bool deleteItem(User admin, std::string itemName);
        bool deleteItem(User admin, int id);

        bool updateQuanity(User admin, int id, std::string itemName, int quantity);
        bool updateQuanity(User admin, int id, int quantity);

        std::optional<Inventory> getItem(User admin, std::string itemName);

    private:
        sqlite3 *db; //the database
};

#endif