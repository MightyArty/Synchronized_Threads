#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <pthread.h>
#include "stack.hpp"
#include <iostream>

#define BACKLOG 10  // how many pending connections queue will hold
#define PORT "3490"
#define amount 5
#define BUFF_SIZE 1024

using namespace std;

struct Stack my_stack;

pthread_t thread_id[amount];
pthread_mutex_t lock;

int newfd[BACKLOG];

void b(char *barak){
    pthread_mutex_lock;
    printf("tata..");
    pthread_mutex_unlock;
}

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

/**
 * @brief our thread function
 * that will replace the fork() function
 * @param new_fd 
 * @return void* 
 */
void *thread_func(void *new_fd){
    printf("new connection %d\n", new_fd);
    int nfd = *(int*)new_fd;
    char buff[BUFF_SIZE];
    bool connected = true;
    int bytes;
    pthread_detach(pthread_self());

    while(connected){
        bytes = recv(nfd, buff, sizeof(buff), NULL);
        if(bytes <= 0){
            perror("recv");
            connected = false;
            pthread_exit(NULL);
            break;
        }
        *(buff+bytes) = '\0';

        pthread_mutex_lock(&lock);

        if(strncmp(buff, "PUSH", 4)){
            push(&my_stack, buff+3);
        }

        else if(strncmp(buff, "POP", 3)){
            pop(&my_stack);
        }

        else if(strncmp(buff, "TOP", 3)){
            top(my_stack);
        }

        else{
            string bb = "did not suceed";
            if(send(nfd, &bb, bb.size(), 0) == -1){
                perror("send");
            }
        }
        pthread_mutex_unlock(&lock);
    }
    printf("client side closed\n");
    close(nfd);
}

int main(void){
    int counter = 0;
    int sockfd, new_fd;  // listen on sock_fd, new connection on new_fd
    struct addrinfo hints, *servinfo, *p;
    struct sockaddr_storage their_addr; // connector's address information
    socklen_t sin_size;
    struct sigaction sa;
    int yes=1;
    char s[INET6_ADDRSTRLEN];
    int rv;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE; // use my IP

    if ((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    // loop through all the results and bind to the first we can
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                p->ai_protocol)) == -1) {
            perror("server: socket");
            continue;
        }
        
        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes,
                sizeof(int)) == -1) {
            perror("setsockopt");
            exit(1);
        }

        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("server: bind");
            continue;
        }

        break;
    }

    freeaddrinfo(servinfo); // all done with this structure

    if (p == NULL)  {
        fprintf(stderr, "server: failed to bind\n");
        exit(1);
    }

    if (listen(sockfd, BACKLOG) == -1) {
        perror("listen");
        exit(1);
    }

    printf("server: waiting for connections...\n");
    unsigned int j = 0;

    while(1){
        sin_size = sizeof (their_addr);
        newfd[j] = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);;
        if(newfd[j] == -1){
            perror("accept");
            continue;
        }
        inet_ntop(their_addr.ss_family, get_in_addr((struct sockaddr *)&their_addr), s, sizeof s);
        printf("server: got connection from %s\n", s);

        pthread_create(&thread_id[j%BACKLOG], NULL, thread_func, &newfd[j]);
        j++;
    }
    return 0;
}