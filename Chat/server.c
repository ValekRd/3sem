#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define MAX_USERS 50
#define MAX_MES_LEN 1000

struct User
{
    in_port_t port;
    in_addr_t ip;
    char* username;
};

struct User users[MAX_USERS];
struct sockaddr_in server, client;

socklen_t clilen;

char message[MAX_MES_LEN];
char *line;
int sockfd;

void sendMes(int currentUser, int i);

int main()
{
    line = malloc(MAX_MES_LEN * sizeof(char));

    bzero(&server, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(51000);
    server.sin_addr.s_addr = htonl(INADDR_ANY);

        if ((sockfd = socket(PF_INET, SOCK_DGRAM, 0)) < 0)
        {
                perror(NULL);
                exit(1);
        }

        else
        {
                printf("Sock fd:%d\n", sockfd);
        }

        if (bind(sockfd, (struct sockaddr*) &server, sizeof(server)) < 0)
        {
            perror(NULL);
            close(sockfd);
            exit(1);
        }

    int User_Pointer = 0;
    int currentUser;



    clilen = sizeof(client);

    ssize_t n = 0;

    while (1)
    {
        bzero(line, MAX_MES_LEN);

        if ((n = recvfrom(sockfd, line, 999, 0, (struct sockaddr*)&client, &clilen)) < 0)
        {
            perror(NULL);
            close(sockfd);
            exit(1);
        }
        currentUser = -1;

        for(int i = 0; i < User_Pointer; i++)
        {
            if((client.sin_port == users[i].port) && (client.sin_addr.s_addr == users[i].ip))
                currentUser = i;
        }
        if(currentUser < 0)
        {
            users[User_Pointer].username = malloc(20 * sizeof(char));

            strcpy(users[User_Pointer].username, line);

            users[User_Pointer].port = client.sin_port;
            users[User_Pointer].ip = client.sin_addr.s_addr;

            printf("%s connected. ip - [%d], port - [%d]\n" ,users[User_Pointer].username, users[User_Pointer].ip, users[User_Pointer].port);

            User_Pointer++;
        }
        else if (line[0] == '!')
        {
            char* private_name;

            private_name = strtok(line, "!");
            line = strtok(NULL, "!");

            for (int j = 0; j < User_Pointer; j++)
            {
                    if (!strcmp(private_name, users[j].username))
                    {
                        sendMes(currentUser, j);
                    }
            }
        }

        else
        {
            for(int i = 0; i < User_Pointer; i++)

            {
                if(i != currentUser)
                {
                    sendMes(currentUser, i);
                }
            }
        }
    }
}

void sendMes(int currentUser, int i)
{
    strcat(message, users[currentUser].username);
    strcat(message, ": ");
    strcat(message, line);

    printf("sending message [%s] to user with ip [%d] and  port [%d]\n", line, users[i].ip, users[i].port);

    bzero(&client, sizeof(client));
    client.sin_family = AF_INET;
    client.sin_port = users[i].port;
    client.sin_addr.s_addr = users[i].ip;

    if (sendto(sockfd, message, strlen(message) + 1, 0, (struct sockaddr*)&client, clilen) < 0)
    {
        perror(NULL);
        close(sockfd);
        exit(1);
    }
    bzero(&message, strlen(message));
}