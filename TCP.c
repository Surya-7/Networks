// PROGRAM:
// tcp_server.c:
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char **argv)
{
    socklen_t len;
    int listenfd, connfd;
    pid_t pid1;
    struct sockaddr_in servaddr, cliaddr;
    char buff[1024];
    int n;
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if (listenfd < 0)
        perror("\nunable to create socket\n\n");
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(5000);
    bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
    listen(listenfd, 0);
    int l;
    for (;;)
    {
        len = sizeof(cliaddr);
        connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &len);
        do
        {
            l = read(connfd, buff, sizeof(buff));
            buff[l] = 0;
            if (strcmp(buff, "exit") != 0)
            {
                printf("\nMessage received from the client is: %s", buff);
                write(connfd, buff, sizeof(buff));
            }
            else
            {
                strcpy(buff, " (chat terminated)");
                printf(" (chat terminated)\n\n");
                write(connfd, buff, sizeof(buff));
                exit(0);
                break;
            }
        } while (strcmp(buff, "exit") != 0);
        break;
    }
    close(listenfd);
    close(connfd);
    return 0;
}

// tcp_client.c :
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

    int
    main(int argc, char **argv)
{
    int sockfd;
    struct sockaddr_in servaddr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        perror("\nsocket error\n");
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(argv[1]);
    servaddr.sin_port = htons(5000);
    connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
    int n;
    socklen_t len;
    char buff[1024], str[1024];
    do
    {
        printf("\nYou: ");
        fgets(str, sizeof(str), stdin);
        write(sockfd, str, strlen(str));
        n = read(sockfd, buff, sizeof(buff));
        buff[n] = 0;
    } while (strcmp(buff, "exit") != 0);

    printf("\n\n");
    close(sockfd);
    return 0;
}