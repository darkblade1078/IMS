#include "../headers/database.h"
#include <iostream>

Database::Database() {
    sqlite3_open("database.db", &db);

    //create the users table if it doesn't exist
    const char *sql1 = "CREATE TABLE IF NOT EXISTS users (id INTEGER PRIMARY KEY AUTOINCRIMENT, username TEXT, password TEXT, perms INTEGER)";

    sqlite3_exec(db, sql1, 0, 0, 0);
}

Database::~Database() {
    sqlite3_close(db);
}

std::optional<User> Database::findUser(std::string username, std::string password) {
    const char *sql = "SELECT * FROM users WHERE username = ? AND password = ?";

    sqlite3_stmt *stmt;

    //prepares the sql code
    sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);

    try {
        //binds the username and password to the sql query
        sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_STATIC);

        //executes the sql code
        sqlite3_step(stmt);

    } catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << std::endl;
        return std::nullopt;
    }

    //gets the permission of the user
    int permission = sqlite3_column_int(stmt, 2);

    //return either a null optional or a User object depending on if the permission exists or nots
    return permissionLevel > 0 ? std::optional<User>(User(username, password, permission)) : std::nullopt;
}

bool Database::createUser(std::string username, std::string password, int perms) {

    const char *sql = "INSERT INTO users (username, password, permission_level) VALUES (?, ?, ?)";

    //our statement
    sqlite3_stmt *stmt;

    //prepares the sql code
    sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);

    try {
        //binds the username, password, and perms to the sql query
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

    return true;
}