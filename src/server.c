#include <stdio.h>      //thu vien nhap xuat trong C
#include <stdlib.h>     //chuyen doi du lieu, quan ly bo nho dong
#include <string.h>     //thao tac voi chuoi ki tu trong c
#include <unistd.h>     // cho sleep, tien trinh delay...
#include <pthread.h>    //tao va quan ly cac luong thread
#include <netinet/in.h> //giao thuc mang TCP/IP
#include "database.h"

#define PORT 8080
#define BUFFER_SIZE 1024
#define MAX_CLIENTS 100

typedef struct
{
    int sock;
    char username[50];
    int logged_in;
} client_t;

client_t clients[MAX_CLIENTS];
pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;

void send_to_client(int client_sock, const char *msg)
{
    send(client_sock, msg, strlen(msg), 0);
}
// guwi tin nhan tu nguoi gui den nguoi nhan, khoa mutex dam bao chon nhieu luong truy cap  mot cach an toan
void broadcast_message(const char *sender, const char *receiver, const char *msg)
{
    pthread_mutex_lock(&clients_mutex);
    int found = 0;
    for (int i = 0; i < MAX_CLIENTS; i++)
    {
        if (clients[i].logged_in && strcmp(clients[i].username, receiver) == 0)
        {
            char buffer[BUFFER_SIZE];
            snprintf(buffer, BUFFER_SIZE, "FROM %s: %s\n", sender, msg);
            send_to_client(clients[i].sock, buffer);
            found = 1;
            break;
        }
    }
    pthread_mutex_unlock(&clients_mutex);

    /////////////////////////////////////////////////
    // ham thonf bao gui tin nhan loi khi user khong online
    if (!found)
    {
        for (int i = 0; i < MAX_CLIENTS; i++)
        {
            if (clients[i].logged_in && strcmp(clients[i].username, sender) == 0)
            {
                send_to_client(clients[i].sock, "SEND_FAILED: User not online\n");
                break;
            }
        }
    }
    else
    {
        //////////////////////////////////////////////////////////////////////
        // ham thong bao da gui tin nhan
        for (int i = 0; i < MAX_CLIENTS; i++)
        {
            if (clients[i].logged_in && strcmp(clients[i].username, sender) == 0)
            {
                send_to_client(clients[i].sock, "SEND_SUCCESS\n");
                save_message(sender, receiver, msg);
                printf("da luu tin nhan"); /////debug
                printf("[Server] %s -> %s: %s\n", sender, receiver, msg);
                break;
            }
        }
    }
}
// xoa client khoi he thong khi ngung ket noi or dang xuat
void remove_client(int client_sock)
{
    // khoa mutex dam bao cho nhieu luong truy cap mot cach an toan
    pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < MAX_CLIENTS; i++)
    {
        if (clients[i].sock == client_sock)
        {
            clients[i].logged_in = 0;
            clients[i].username[0] = '\0';
            clients[i].sock = -1;
            break;
        }
    }
    pthread_mutex_unlock(&clients_mutex);
}

void *handle_client(void *arg)
{
    int client_sock = *(int *)arg;
    free(arg);

    char buffer[BUFFER_SIZE];
    char username[50], password[50];
    int logged_in = 0;

    // add client
    // khoa mutex dam bao chon nhieu luong truy mot cach an toan
    pthread_mutex_lock(&clients_mutex);
    int i;
    for (i = 0; i < MAX_CLIENTS; i++)
    {
        if (clients[i].sock == -1)
        {
            clients[i].sock = client_sock;
            clients[i].logged_in = 0;
            clients[i].username[0] = '\0';
            break;
        }
    }
    pthread_mutex_unlock(&clients_mutex);

    while (1)
    {
        int len = recv(client_sock, buffer, BUFFER_SIZE - 1, 0);
        if (len <= 0)
            break; // Client shutdown
        buffer[len] = '\0';
        // dang ki user moi
        if (strncmp(buffer, "REGISTER", 8) == 0)
        {
            sscanf(buffer + 9, "%s %s", username, password);
            if (register_user(username, password))
                send(client_sock, "REGISTER_SUCCESS\n", 17, 0);
            else
                send(client_sock, "REGISTER_FAILED\n", 16, 0);
        }
        else if (strncmp(buffer, "LOGIN", 5) == 0)
        {
            sscanf(buffer + 6, "%s %s", username, password);
            if (login_user(username, password))
            {
                send(client_sock, "LOGIN_SUCCESS\n", 14, 0);
                // khoa mutex dam bao chon nhieu luong truy mot cach an toan
                logged_in = 1;
                pthread_mutex_lock(&clients_mutex);
                strcpy(clients[i].username, username);
                clients[i].logged_in = 1;
                pthread_mutex_unlock(&clients_mutex);
                // lay lich su tin nhan cu ngay sau khi dang nhap thanh cong
                send_history(username, client_sock);
            }
            else
            {
                send(client_sock, "LOGIN_FAILED\n", 13, 0);
            }
        }
        else if (strncmp(buffer, "SEND", 4) == 0)
        {
            if (!logged_in)
            {
                send(client_sock, "ERROR: Please login first\n", 26, 0);
                continue;
            }
            // Cú pháp: SEND <username> <message>
            char to_user[50], message[BUFFER_SIZE];
            if (sscanf(buffer + 5, "%s %[^\n]", to_user, message) < 2)
            {
                send(client_sock, "ERROR: Invalid SEND format\n", 27, 0);
                continue;
            }
            broadcast_message(username, to_user, message);
        }
        else
        {
            send(client_sock, "UNKNOWN_COMMAND\n", 16, 0);
        }
    }

    remove_client(client_sock);
    close(client_sock);
    return NULL;
}

int main()
{
    if (!init_database())
    {
        fprintf(stderr, "Database init failed\n");
        return 1;
    }
    // tao cac thread tuong ung user toi da 99
    for (int i = 0; i < MAX_CLIENTS; i++)
    {
        clients[i].sock = -1;
        clients[i].logged_in = 0;
        clients[i].username[0] = '\0';
    }
    // tao socket va thiet lap dia chi
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server_addr = {0};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;
    // gan socket voi dia chi IP va cong
    bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    // lang nghe ket noi toi da 10 ket noi o hang doi
    listen(server_fd, 10);
    printf("Server running on port %d...\n", PORT);

    while (1)
    {
        // chap nhan clien ket noi
        int client_sock = accept(server_fd, NULL, NULL);
        int *pclient = malloc(sizeof(int));
        *pclient = client_sock;
        pthread_t tid;
        // tao luong xu ly
        pthread_create(&tid, NULL, handle_client, pclient);
        pthread_detach(tid);
    }

    close(server_fd); // ket thuc
    return 0;
}
