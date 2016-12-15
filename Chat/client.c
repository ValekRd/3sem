#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>

#define MAX_LEN 100
#define MAX_NAME 100

int main(int argc, char** argv)
{
    int sockfd;
    struct sockaddr_in servaddr, cliaddr;

    if (argc != 2)
    {
        printf("Write: ./<program name> <IP address>\n");
        exit(1);
    }
    if ((sockfd = socket(PF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror(NULL);
        exit(1);
    }

    bzero(&cliaddr, sizeof(cliaddr));
    cliaddr.sin_family = AF_INET;
    cliaddr.sin_port = htons(0);
    cliaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(sockfd, (struct sockaddr*)&cliaddr, sizeof(cliaddr)) < 0)
    {
        perror(NULL);
        close(sockfd);
        exit(1);
    }
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(51000);
    if (inet_aton(argv[1], &servaddr.sin_addr) == 0)
    {
        printf("Wrong IP address\n");
        close(sockfd);
        exit(1);
    }

    char User_Name[20];
    printf("Welcome to Chat! Enter your username:\n");
    gets(User_Name);

    if (sendto(sockfd, User_Name, strlen(User_Name), 0, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0)
    {
        perror(NULL);
        close(sockfd);
        exit(1);
    }
    pid_t pid = fork();
    if(pid == 0)
    {
        while(1)
        {
            fgets(message, MAX_NAME, stdin);
            if (sendto(sockfd, message, strlen(message) + 1, 0, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0)
            {
                perror(NULL);
                close(sockfd);
                exit(1);
            }
            bzero(&message, strlen(message));
        }
    }
    else
    {
        int n;
        char recvline[MAX_LEN];
        while(1)
        {
            if ((n = recvfrom(sockfd, recvline, MAX_LEN, 0, (struct sockaddr*) NULL, NULL)) < 0)
            {
                perror(NULL);
                close(sockfd);
                exit(1);
            }
            printf("%s", recvline);
        }
    }
}