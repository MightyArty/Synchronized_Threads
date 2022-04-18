#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <fstream>
#include <strings.h>
#include <stdlib.h>
#include <string>
#include <pthread.h>
#define BUFFSIZE 1024

#include <string>
#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif
#include <bits/stdc++.h>
#include <iostream>
#include <cstdlib>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <thread>
class Stack
{
public:
    char *data;
    Stack *next;
};
pthread_mutex_t lock;
Stack *my_stack;
void *task1(void *);
Stack *newNode(char *data)
{
    Stack *stack = new Stack();
    stack->data = data;
    puts(stack->data);
    stack->next = NULL;
    return stack;
}
int isEmpty(Stack *root)
{
    return !root;
}
char *pop(Stack **root)
{
    if (isEmpty(*root))
        return (char *)"Empty";
    pthread_mutex_lock(&lock);

    Stack *temp = *root;
    *root = (*root)->next;
    char *popped = temp->data;
    free(temp);
    pthread_mutex_unlock(&lock);

    return popped;
}
void push(Stack **root, char *data)
{
    pthread_mutex_lock(&lock);
    puts(data);
    Stack *Stack = newNode(data);
    Stack->next = *root;
    *root = Stack;
    pthread_mutex_unlock(&lock);
}
char *top(Stack *root)
{
    char *s;
    if (isEmpty(root))
        s = (char *)"Empty";
    else
        s = root->data;
    return root->data;
}
int main(int argc, char *argv[])
{
    int pId, portNo, listenFd;
    socklen_t len; // store size of the address
    bool loop = false;
    struct sockaddr_in svrAdd, clntAdd;

    pthread_t threadA[3];

    if (argc < 2)
    {
        std::cerr << "Syntam : ./server <port>" << std::endl;
        return 0;
    }

    portNo = atoi(argv[1]);

    if ((portNo > 65535) || (portNo < 2000))
    {
        std::cerr << "Please enter a port number between 2000 - 65535" << std::endl;
        return 0;
    }

    // create socket
    listenFd = socket(AF_INET, SOCK_STREAM, 0);

    if (listenFd < 0)
    {
        std::cerr << "Cannot open socket" << std::endl;
        return 0;
    }

    bzero((char *)&svrAdd, sizeof(svrAdd));

    svrAdd.sin_family = AF_INET;
    svrAdd.sin_addr.s_addr = INADDR_ANY;
    svrAdd.sin_port = htons(portNo);

    // bind socket
    if (bind(listenFd, (struct sockaddr *)&svrAdd, sizeof(svrAdd)) < 0)
    {
        std::cerr << "Cannot bind" << std::endl;
        return 0;
    }

    listen(listenFd, 5);

    int noThread = 0;

    while (noThread <= 3)
    {
        std::cout << "Listening" << std::endl;
        socklen_t len = sizeof(clntAdd);
        // this is where client connects. svr will hang in this mode until client conn
        int connFd = accept(listenFd, (struct sockaddr *)&clntAdd, &len);

        if (connFd < 0)
        {
            std::cerr << "Cannot accept connection" << std::endl;
            return 0;
        }
        else
        {
            std::cout << "Connection successful" << std::endl;
        }

        pthread_create(&threadA[noThread], NULL, task1, (void *)&connFd);
        noThread++;
    }

    for (int i = 0; i < 3; i++)
    {
        pthread_join(threadA[i], NULL);
    }
}
void *task1(void *dummyPt)
{
    int sock = *((int *)dummyPt);
    std::cout << "Thread No: " << pthread_self() << std::endl;
    std::cout << sock << std::endl;
    char r[BUFFSIZE] = {0};
    char *w;
    while (true)
    {
        if (read(sock, r, BUFFSIZE) == -1)
            puts("erro");

        if (strncmp(r, "PUSH", 4) == 0)
        {
            push(&my_stack, r + 5);
            puts(top(my_stack));
            write(sock, "pushed", 6);
        }
        else if (strncmp(r, "POP", 3) == 0)
        {
            puts("POP");
            w = pop(&my_stack);
            int len = strlen(w);
            write(sock, w, len);
        }
        else if (strncmp(r, "TOP", 3) == 0)
        {
            puts("TOP");
            w = top(my_stack);
            int len = strlen(w);
            write(sock, w, len);
        }
        else if (strncmp(r, "exit", 4) == 0)
        {
            write(sock, "succ", 4);
            close(sock);
            return 0;
        }
    }

    std::cout << "\nClosing thread and conn" << std::endl;
    write(sock, "exit", 4);
    close(sock);
    return 0;
}
