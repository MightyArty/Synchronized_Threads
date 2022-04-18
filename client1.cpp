#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <regex.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>

#define max_length 256
#define max_buff 1024
#define BUFFSIZE 1024
int sock;
bool connected = true;

void sig_handler(int signum){
    switch (signum)
    {
    case SIGTSTP: // dad is dead
        printf("I'm the first signal..\n");
        close(sock);
    case SIGINT: // raise by user signal
        printf("I'm the second signal\n");
        close(sock);
    default:
        exit(1);
        close(sock);
    }
}


int main()
{
    signal(SIGINT, sig_handler);
    signal(SIGTSTP, sig_handler);
    struct sockaddr_in add;
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        printf("error in connectiong...\n");
        return -1;
    }
    // printf("success in creating socket\n");

    // do we need memset??
    add.sin_family = AF_INET;
    add.sin_port = 3000;
    add.sin_addr.s_addr = INADDR_ANY;

    int status = connect(sock, (struct sockaddr *)&add, sizeof(add));
    if (status == -1)
    {
        printf("Connection error...\n");
        return -1;
    }
    // fprintf(stdout, "connected to port: %d", add.sin_port);
    // dup2(1,123);
    // dup2(sock,1);

    while(connected){
        char buff [BUFFSIZE] = {0};
        fgets(buff,sizeof(buff),stdin);
        write(sock, buff, sizeof(buff));
        bzero(buff, sizeof(buff));
        read(sock, buff, sizeof(buff));
        fprintf(stdout, "connected to port: %s", buff);
    }

    return 1;
}