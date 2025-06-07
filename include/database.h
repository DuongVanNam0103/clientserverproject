// database.h
#ifndef DATABASE_H
#define DATABASE_H

int init_database();
int register_user(const char *username, const char *password);
int login_user(const char *username, const char *password);
///////////////////////////////////////////////////
int save_message(const char *sender, const char *receiver, const char *msg);
int send_history(const char *username, int client_sock);

#endif
