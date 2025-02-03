#include "../headers/database.h"
#include "../headers/utilities.h"
#include <iostream>
#include <optional>

Database::Database() {
    sqlite3_open("database.db", &db);

    //create the users table if it doesn't exist
    const char *sql1 = "CREATE TABLE IF NOT EXISTS users (username TEXT PRIMARY KEY, password TEXT, permission_level INTEGER)";


    //create the inventory table if it doesn't exist
    const char *sql2 = "CREATE TABLE IF NOT EXISTS inventory (id INTEGER PRIMARY KEY AUTOINCREMENT, item_name TEXT, quantity INTEGER)";

    sqlite3_exec(db, sql1, 0, 0, 0);
    sqlite3_exec(db, sql2, 0, 0, 0);
}

Database::~Database() {
    sqlite3_close(db);
}

int Database::getUserCount() {
    //sql code to check if any users exist
    const char *sql = "SELECT COUNT(*) FROM users";

    //our statement
    sqlite3_stmt *stmt;

    //prepares the sql code
    sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);

    //executes the sql code
    sqlite3_step(stmt);

    //gets the count of users
    int count = sqlite3_column_int(stmt, 0);

    //finalizes the statement so we don't get memory leaks
    sqlite3_finalize(stmt);

    //returns true if any users exist
    return count;
}

bool Database::createFirstUser(std::string username, std::string password) {
    clearScreen();  //clear the screen

    const char *sql = "INSERT INTO users (username, password, permission_level) VALUES (?, ?, ?)";

    //our statement
    sqlite3_stmt *stmt;

    //prepares the sql code
    sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);

    try {
        //binds the username, password, and permission level to the sql code
        sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_int(stmt, 3, 0);

        //executes the sql code
        sqlite3_step(stmt);

    } catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << std::endl;
        return false;
    }

    //finalizes the statement so we don't get memory leaks
    sqlite3_finalize(stmt);

    std::cout << username << ", has been created" << std::endl;
    std::cout << "username: " << username << std::endl;
    std::cout << "password: " << password << std::endl;

    return true;
}

bool Database::createUser(User admin, std::string username, std::string password, int permissionLevel) {
    clearScreen();  //clear the screen

    //get the permission level of the current user
    int adminPermissionLevel = admin.getPermissionLevel();

    //check if the admin is the owner or an admin
    if(adminPermissionLevel > 1) {
        std::cout << "Error: " << "You do not have permission to create users" << std::endl;
        return false;
    }

    //check if the user is being created is a owner/admin and if the current admin has owner perms
    if(adminPermissionLevel == 1 && (permissionLevel == 1 || permissionLevel == 0)) {
        std::cout << "Error: " << "You cannot create a user with owner or admin perms" << std::endl;
        return false;
    }

    //checks if the user is being created is being given owner perms
    if(permissionLevel == 0) {
        std::cout << "Error: " << "There can only be one owner" << std::endl;
        return false;
    }

    const char *sql = "INSERT INTO users (username, password, permission_level) VALUES (?, ?, ?)";

    //our statement
    sqlite3_stmt *stmt;

    //prepares the sql code
    sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);

    try {
        //binds the username, password, and permission level to the sql code
        sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_int(stmt, 3, permissionLevel);

        //executes the sql code
        sqlite3_step(stmt);
    } catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << std::endl;
        return false;
    }

    //finalizes the statement so we don't get memory leaks
    sqlite3_finalize(stmt);

    std::cout << username << ", has been created" << std::endl;
    std::cout << "username: " << username << std::endl;
    std::cout << "password: " << password << std::endl;

    return true;
}

bool Database::deleteUser(User admin, std::string username) {
    clearScreen();  //clear the screen

    //get the permission level of the current user
    int adminPermissionLevel = admin.getPermissionLevel();

    //check if the admin is actually the owner or an admin
    if(adminPermissionLevel > 1) {
        std::cout << "Error: " << "You do not have permission to delete users" << std::endl;
        return false;
    }

    //check if there is less than 2 users (so we don't delete that last user which should be the owner) (otherwise we have no users left and that is a huge no no) 
    if(this->getUserCount() < 2) {
        std::cout << "Error: " << "You are the only user left" << std::endl;
        return false;
    }

    const char *sqlCheck = "SELECT * FROM users WHERE username = ?";

    //our statement
    sqlite3_stmt *stmt1;

    //prepares the sql code
    sqlite3_prepare_v2(db, sqlCheck, -1, &stmt1, nullptr);

    //binds the username to the sql code
    sqlite3_bind_text(stmt1, 1, username.c_str(), -1, SQLITE_STATIC);

    //executes the sql code
    sqlite3_step(stmt1);

    //gets the permission level of the user
    int userPermissionLevel = sqlite3_column_int(stmt1, 2);

    //finalizes statement 1 so we don't get memory leaks
    sqlite3_finalize(stmt1);

    //checks if the user being deleted is the owner
    if(userPermissionLevel == 0) {
        std::cout << "Error: " << "You cannot delete the owner of the database" << std::endl;
        return false;
    }

    //checks if the user being deleted is an admin and if the current admin has owner perms
    if (userPermissionLevel == 1 && adminPermissionLevel != 0) {
        std::cout << "Error: " << "You cannot delete another admin" << std::endl;
        return false;
    }

    //sql code to delete a user
    const char *sql = "DELETE FROM users WHERE username = ?";

    //our statement
    sqlite3_stmt *stmt2;

    //prepares the sql code
    sqlite3_prepare_v2(db, sql, -1, &stmt2, nullptr);

    //binds the username to the sql code
    sqlite3_bind_text(stmt2, 1, username.c_str(), -1, SQLITE_STATIC);

    //executes the sql code
    try {
        sqlite3_step(stmt2);
    }
    catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << std::endl;
        return false;
    }

    //finalizes the statement so we don't get memory leaks
    sqlite3_finalize(stmt2);

    return true;
}

bool Database::changePermissions(User admin, std::string username, int permissionLevel) {
    clearScreen();  //clear the screen

    //get the permission level of the admin
    int adminPermissionLevel = admin.getPermissionLevel();

    //check if the admin is actually the owner or an admin
    if(adminPermissionLevel > 1) {
        std::cout << "Error: " << "You do not have permission to change permissions" << std::endl;
        return false;
    }

    //checks if the user is being given owner perms
    if(permissionLevel == 0) {
        std::cout << "Error: " << "There can only be one owner" << std::endl;
        return false;
    }

    //checks if the user is being given admin perms by an admin
    if(adminPermissionLevel == 1 && permissionLevel == 1) {
        std::cout << "Error: " << "Only the owner can give someone admin perms" << std::endl;
        return false;
    }

    const char *sql = "UPDATE users SET permission_level = ? WHERE username = ?";

    sqlite3_stmt *stmt;

    sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);

    sqlite3_bind_int(stmt, 1, permissionLevel);
    sqlite3_bind_text(stmt, 2, username.c_str(), -1, SQLITE_STATIC);
    
    try {
        //executes the sql code
        sqlite3_step(stmt);
    } catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << std::endl;
        return false;
    }

    return true;
}

bool Database::updatePassword(User admin, std::string username, std::string newPassword) {
    clearScreen();  //clear the screen

    //get the permission level of the current user
    int adminPermissionLevel = admin.getPermissionLevel();

    //check if the admin is actually the owner or an admin
    if(adminPermissionLevel > 1) {
        std::cout << "Error: " << "You do not have permission to update passwords" << std::endl;
        return false;
    }

    //update the password of the user
    const char *sql = "UPDATE users SET password = ? WHERE username = ?";

    sqlite3_stmt *stmt;

    sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);

    sqlite3_bind_text(stmt, 1, newPassword.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, username.c_str(), -1, SQLITE_STATIC);
    
    try {
        //executes the sql code
        sqlite3_step(stmt);
    } catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << std::endl;
        return false;
    }

    return true;
}

std::optional<User> Database::login(std::string username, std::string password) {
    //sql code to check if the username and password match
    const char *sql = "SELECT * FROM users WHERE username = ? AND password = ?";

    //our statement
    sqlite3_stmt *stmt;

    //prepares the sql code
    sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);

    //binds the username and password to the sql code
    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_STATIC);

    try {
        //executes the sql code
        sqlite3_step(stmt);
    } catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << std::endl;
        return std::nullopt;
    }

    //gets the permission level of the user
    int permissionLevel = sqlite3_column_int(stmt, 2);

    //finalizes the statement so we don't get memory leaks
    sqlite3_finalize(stmt);

        //prepares the sql code
    sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);

    //binds the username to the sql code
    sqlite3_bind_text(stmt, 0, username.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 1, password.c_str(), -1, SQLITE_STATIC);

    //ternary operator to return a user object if the username and password match, otherwise return null
    return permissionLevel > 0 ? std::optional<User>(User(username, password, permissionLevel)) : std::nullopt;
}

bool Database::createItem(User admin, std::string itemName, int quantity) {
    clearScreen();  //clear the screen

    //get the permission level of the current user
    int adminPermissionLevel = admin.getPermissionLevel();

    //check if the admin is actually the owner or an admin
    if(adminPermissionLevel > 1) {
        std::cout << "Error: " << "You do not have permission to create items" << std::endl;
        return false;
    }

    const char *sql = "INSERT INTO inventory (item_name, quantity) VALUES (?, ?)";

    //our statement
    sqlite3_stmt *stmt;

    //prepares the sql code
    sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);

    try {
        //binds the item name and quantity to the sql code
        sqlite3_bind_text(stmt, 1, itemName.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_int(stmt, 2, quantity);

        //executes the sql code
        sqlite3_step(stmt);
    } catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << std::endl;
        return false;
    }

    //finalizes the statement so we don't get memory leaks
    sqlite3_finalize(stmt);

    return true;
}

bool deleteItem(User admin, std::string itemName) {
    
}