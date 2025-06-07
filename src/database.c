#include <stdio.h>       //thu vien nhap xuat trong C
#include <stdlib.h>      //chuyen doi du lieu, quan ly bo nho dong
#include <string.h>      //thao tac voi chuoi ki tu trong c
#include <sqlite3.h>     //thu vien lam viec voi SQL
#include <openssl/sha.h> //su dung trong bao mat, tinh gia tri SHI
#include <sys/socket.h>  // cho send, dung cho socket
#include <unistd.h>      // cho sleep, tin trinh delay...
#include "database.h"
#define BUFFER_SIZE 1024

sqlite3 *db;
// tao file chat.db
int init_database()
{
    int rc = sqlite3_open("data/chat.db", &db);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        return 0;
    }
    // tao bang luu user va password dang ki
    const char *sql_users =
        "CREATE TABLE IF NOT EXISTS users ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "username TEXT UNIQUE,"
        "password_hash TEXT);";

    char *errmsg = NULL;
    rc = sqlite3_exec(db, sql_users, 0, 0, &errmsg);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "SQL error: %s\n", errmsg);
        sqlite3_free(errmsg);
        return 0;
    }

    /// tao bang cho luu lich su tin  tin nhan
    const char *sql_msgs =
        "CREATE TABLE IF NOT EXISTS messages ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "sender TEXT,"
        "receiver TEXT,"
        "msg TEXT,"
        "timestamp DATETIME DEFAULT CURRENT_TIMESTAMP);";
    rc = sqlite3_exec(db, sql_msgs, 0, 0, &errmsg);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "SQL error: %s\n", errmsg);
        sqlite3_free(errmsg);
        return 0;
    }
    return 1;
}
// nhan mot gia tri mat khau va tra ve chuoi ki tu gia tri bam SHA
char *hash_password(const char *password)
{
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256((const unsigned char *)password, strlen(password), hash);

    static char output[65];
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++)
        sprintf(output + i * 2, "%02x", hash[i]);
    return output;
}
// dang ki nguoi dung moi va luu tru nguoi dung moi vao bang
int register_user(const char *username, const char *password)
{
    const char *sql = "INSERT INTO users (username, password_hash) VALUES (?, ?);";
    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK)
        return 0;

    sqlite3_bind_text(stmt, 1, username, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, hash_password(password), -1, SQLITE_STATIC);

    int rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    return rc == SQLITE_DONE;
}
// tim ten user da dang ki tu bang da luu
int login_user(const char *username, const char *password)
{
    const char *sql = "SELECT password_hash FROM users WHERE username = ?;";
    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK)
        return 0;

    sqlite3_bind_text(stmt, 1, username, -1, SQLITE_STATIC);
    int rc = sqlite3_step(stmt);
    if (rc != SQLITE_ROW)
        return 0;

    const char *stored_hash = (const char *)sqlite3_column_text(stmt, 0);
    int result = strcmp(stored_hash, hash_password(password)) == 0;
    sqlite3_finalize(stmt);
    return result;
}
// ham luu lich su tin nhan vao bang
int save_message(const char *sender, const char *receiver, const char *msg)
{
    const char *sql = "INSERT INTO messages (sender, receiver, msg) VALUES (?, ?, ?);";
    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK)
        return 0;
    sqlite3_bind_text(stmt, 1, sender, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, receiver, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, msg, -1, SQLITE_STATIC);
    int rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    return rc == SQLITE_DONE;
}
// lay lich su tin nhan cua user khi dang nhap thanh cong
int send_history(const char *username, int client_sock)

{
    printf("Sending history for user %s\n", username); // DEBUG
    const char *sql = "SELECT sender, receiver, msg, timestamp FROM messages "
                      "WHERE sender = ? OR receiver = ? ORDER BY id ASC;";
    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK)
        return 0;
    sqlite3_bind_text(stmt, 1, username, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, username, -1, SQLITE_STATIC);

    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        const char *sender = (const char *)sqlite3_column_text(stmt, 0);
        const char *receiver = (const char *)sqlite3_column_text(stmt, 1);
        const char *msg = (const char *)sqlite3_column_text(stmt, 2);
        const char *timestamp = (const char *)sqlite3_column_text(stmt, 3);

        char buffer[BUFFER_SIZE];
        snprintf(buffer, sizeof(buffer), "[%s] %s -> %s: %s\n", timestamp, sender, receiver, msg);
        send(client_sock, buffer, strlen(buffer), 0);
        usleep(50000); // time delay
    }
    sqlite3_finalize(stmt);
    return 1;
}
