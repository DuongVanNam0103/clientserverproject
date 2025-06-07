#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int sock;

void *recv_handler(void *arg)
{
    char buffer[BUFFER_SIZE];
    while (1)
    {
        int len = recv(sock, buffer, BUFFER_SIZE - 1, 0);
        if (len <= 0)
        {
            printf("\nDisconnected from server.\n");
            exit(0);
        }
        buffer[len] = '\0';
        printf("\nServer: %s\n", buffer);
        fflush(stdout);
    }
    return NULL;
}
////////////////////////////////
void show_main_menu()
{
    printf("\nMenu:\n");
    printf("1. REGISTER\n");
    printf("2. LOGIN\n");
    printf("3. SEND\n");
    printf("Select an option: ");
}
/////////////////////////////////////////
void register_user_in()
{
    char username[50], password[50];

    printf("\nEnter username for registration: ");
    gets(username);
    // fgets(username, sizeof(username), stdin);
    // username[strcspn(username, "\n")] = 0; //

    printf("Enter password for registration: ");
    gets(password);
    // fgets(password, sizeof(password), stdin);
    // password[strcspn(password, "\n")] = 0; // Remove newline character

    char buffer[BUFFER_SIZE];
    snprintf(buffer, sizeof(buffer), "REGISTER %s %s", username, password);
    send(sock, buffer, strlen(buffer), 0);
}
//////////////////////////////////////////////////////////
void login_user_in()
{
    char username[50], password[50];

    printf("\nEnter username for login: ");
    gets(username);
    // fgets(username, sizeof(username), stdin);
    // username[strcspn(username, "\n")] = 0; // Remove newline character

    printf("Enter password for login: ");
    gets(password);
    // fgets(password, sizeof(password), stdin);
    // password[strcspn(password, "\n")] = 0; // Remove newline character

    char buffer[BUFFER_SIZE];
    snprintf(buffer, sizeof(buffer), "LOGIN %s %s", username, password);
    send(sock, buffer, strlen(buffer), 0);
}
/////////////////////////////////////////////////////////////
void send_message()
{
    char to_user[50], message[BUFFER_SIZE];

    printf("\nEnter recipient's username: ");
    gets(to_user);
    // fgets(to_user, sizeof(to_user), stdin);
    // to_user[strcspn(to_user, "\n")] = 0; // Remove newline character

    printf("Enter your message: ");
    gets(message);
    // fgets(message, sizeof(message), stdin);
    // message[strcspn(message, "\n")] = 0; // Remove newline character

    char buffer[BUFFER_SIZE];
    snprintf(buffer, sizeof(buffer), "SEND %s %s", to_user, message);
    send(sock, buffer, strlen(buffer), 0);
}
////////////////////////////////////////////////////////////////////////
int main()
{
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        perror("socket failed");
        return 1;
    }

    struct sockaddr_in server_addr = {0};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);

    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("Connection failed");
        return 1;
    }

    pthread_t recv_thread;
    if (pthread_create(&recv_thread, NULL, recv_handler, NULL) != 0)
    {
        perror("pthread_create failed");
        close(sock);
        return 1;
    }

    // char buffer[BUFFER_SIZE];
    // while (1)
    // {
    //     printf("Enter command (REGISTER / LOGIN / SEND): ");
    //     fflush(stdout);
    //     if (!fgets(buffer, BUFFER_SIZE, stdin))
    //         break;

    //     // Loại bỏ ký tự newline cuối dòng (nếu có)
    //     size_t len = strlen(buffer);
    //     if (len > 0 && buffer[len - 1] == '\n')
    //         buffer[len - 1] = '\0';

    //     send(sock, buffer, strlen(buffer), 0);
    // }

    while (1)
    {
        show_main_menu();
        int num;
        scanf("%d", &num);
        getchar();

        switch (num)
        {
        case 1:
            register_user_in();
            break;
        case 2:
            login_user_in();
            break;
        case 3:
            send_message();
            break;
        default:
            printf("Invalid choice, please try again.\n");
            break;
        }
    }

    close(sock);
    return 0;
}
