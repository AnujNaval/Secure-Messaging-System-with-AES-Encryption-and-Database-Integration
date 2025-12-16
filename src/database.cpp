#include "database.h"
#include <sqlite3.h>
#include <stdexcept>
#include <string>

void setup_database() {
    sqlite3* db;
    const char* sql = R"(
        CREATE TABLE IF NOT EXISTS messages (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            username BLOB NOT NULL,
            message BLOB NOT NULL,
            iv BLOB NOT NULL,
            timestamp DATETIME DEFAULT CURRENT_TIMESTAMP
        )
    )";

    if (sqlite3_open("secure_messages.db", &db) != SQLITE_OK)
        throw std::runtime_error("Failed to open SQLite database");

    char* errmsg;
    if (sqlite3_exec(db, sql, nullptr, nullptr, &errmsg) != SQLITE_OK) {
        std::string error = errmsg;
        sqlite3_free(errmsg);
        sqlite3_close(db);
        throw std::runtime_error("Failed to create table: " + error);
    }

    sqlite3_close(db);
}

void store_message(const std::string& username, const std::string& message,
                   const unsigned char* iv) {
    sqlite3* db;
    if (sqlite3_open("secure_messages.db", &db) != SQLITE_OK)
        throw std::runtime_error("Failed to open SQLite database");

    const char* sql = "INSERT INTO messages (username, message, iv) VALUES (?, ?, ?)";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        sqlite3_close(db);
        throw std::runtime_error("Failed to prepare statement");
    }

    sqlite3_bind_text(stmt, 1, username.c_str(), username.size(), SQLITE_TRANSIENT);
    sqlite3_bind_blob(stmt, 2, message.c_str(), message.size(), SQLITE_TRANSIENT);
    sqlite3_bind_blob(stmt, 3, iv, 16, SQLITE_TRANSIENT);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        throw std::runtime_error("Failed to execute statement");
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
}
