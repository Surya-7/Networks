PROGRAM : server.c :
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>

    void
    error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        printf("Usage: ./%s [PORT NUMBER]\nExiting..", argv[0]);
        exit(1);
    }
    int listenfd, connfd, portno, n;
    char buff[1024];

    struct sockaddr_in cliaddr, servaddr;
    socklen_t len;

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if (listenfd < 0)
        error("\nError creating socket\n");
    bzero((char *)&servaddr, sizeof(servaddr));
    portno = atoi(argv[1]);

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(portno);

    if (bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
        error("\nCould not bind\n");

    listen(listenfd, 0);

    len = sizeof(cliaddr);
    connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &len);
    if (connfd < 0)
        error("\nError on accept\n");

    for (;;)
    {
        bzero(buff, 1024);
        n = read(connfd, buff, sizeof(buff));
        if (n < 0)
            error("\nError on reading\n");
        printf("Client: %s\n", buff);
        bzero(buff, sizeof(buff));
        fgets(buff, sizeof(buff), stdin);

        n = write(connfd, buff, strlen(buff));
        if (n < 0)
            error("\nError on writing\n");

        if (strncmp(buff, "Bye", sizeof(buff)) == 0 || strncmp(buff, "bye", sizeof(buff)) == 0)
            break;
    }
    close(connfd);
    close(listenfd);
    return 0;
}

client.c :
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
    void
    error(const char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char **argv)
{
    if (argc < 3)
    {
        printf("Usage: ./%s [HOSTNAME] [PORT NUMBER]\nExiting..", argv[0]);
        exit(0);
    }
    int sockfd, portno, n;
    struct sockaddr_in servaddr;
    struct hostent *server;

    char buff[1024];
    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("\nError opening socket\n");

    server = gethostbyname(argv[1]);
    if (server == NULL)
    {
        printf("No such host\n");
        exit(1);
    }
    bzero((char *)&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&servaddr.sin_addr.s_addr, server->h_length);
    servaddr.sin_port = htons(portno);

    if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
        error("Connection failed\n");

    for (;;)
    {
        bzero(buff, 1024);
        fgets(buff, sizeof(buff), stdin);
        n = write(sockfd, buff, strlen(buff));
        if (n < 0)
            error("\nError on writing\n");

        bzero(buff, 1024);
        n = read(sockfd, buff, sizeof(buff));
        if (n < 0)
            error("\nError on reading\n");
        printf("Server: %s\n", buff);

        if (strncmp(buff, "Bye", sizeof(buff)) == 0 || strncmp(buff, "bye", sizeof(buff)) == 0)
            break;
    }

    close(sockfd);
    return 0;
}