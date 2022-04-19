#include "server.hpp"
#define em 5
void red()
{
    printf("\033[1;31m");
}
void yellow()
{
    printf("\033[1;33m");
}
void reset()
{
    printf("\033[0m");
}
void free(Stack **root)
{
    while (*root)
    {
        Stack *temp = *root;
        *root = (*root)->next;
        free(temp->data);
        delete temp;
    }
    std::cout << "free" << std::endl;
}
void sig_handler(int signum)
{
    free(my_stack);
    switch (signum)
    {
    case SIGTSTP:
        red();
        printf("I'm the first signal..\n");
    case SIGINT:
        yellow();
        printf("I'm the second signal, trying to divide\n");
    default:

        close(listenFd);
        reset();
        exit(1);
    }
}
Stack *newNode(char *data)
{
    Stack *stack = new Stack();
    stack->data = strcpy((char *)malloc(BUFFSIZE), data);
    stack->next = NULL;
    return stack;
}
int isEmpty(Stack *root)
{
    return !root;
}
Stack *pop(Stack **root)
{
    if (isEmpty(*root))
    {
        return NULL;
    }
    pthread_mutex_lock(&lock);
    Stack *temp = *root;
    *root = (*root)->next;
    pthread_mutex_unlock(&lock);
    size--;
    return temp;
}
void push(Stack **root, char *data)
{
    pthread_mutex_lock(&lock);
    size++;
    Stack *Stack = newNode(data);
    Stack->next = *root;
    *root = Stack;
    pthread_mutex_unlock(&lock);
}
char *top(Stack *root)
{
    char *s;
    if (isEmpty(root))
    {
        return NULL;
    }
    pthread_mutex_lock(&lock);
    s = root->data;
    pthread_mutex_unlock(&lock);
    return s;
}
int server()
{

    // if (argc < 2)
    // {
    //     std::cerr << "Syntam : ./server <port>" << std::endl;
    //     return 0;
    // }

    // portNo = atoi(argv[1]);
    portNo = htons(3006);
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

    if (listen(listenFd, 5) == -1)
    {
        printf("\n listen has failed\n");
        return 0;
    }

    if (pthread_mutex_init(&lock, NULL) != 0)
    {
        printf("\n mutex init has failed\n");
        return 0;
    }

    return 1;
}
int main(int argc, char *argv[])
{

    signal(SIGINT, sig_handler);
    signal(SIGTSTP, sig_handler);
    if (!server())
        return 0;
    while (noThread < 3)
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
        int a[]{connFd, noThread};
        int error = pthread_create(&threadA[noThread], NULL, task1, (void *)&a);
        if (error != 0)
            printf("\nThread can't be created :[%s]",
                   strerror(error));
        noThread++;
    }

    for (int i = 0; i < 3; i++)
    {
        pthread_join(threadA[i], NULL);
    }
    free(&my_stack);
    // for (auto &thread : threadB) // access by reference to avoid copying
    // {
    //     pthread_join(thread, NULL);
    // }
}
void *task1(void *dummyPt)
{
    int sock = *((int *)dummyPt);
    int numberThread = *((int *)dummyPt + 1);
    std::cout << "Thread No: " << pthread_self() << " Socket No " << sock << " NumberThread " << numberThread << std::endl;

    while (true)
    {
        char *writer = 0;
        char reader[BUFFSIZE] = {0};
        bzero(reader, BUFFSIZE);
        if (read(sock, reader, BUFFSIZE) == -1)
        {
            puts("erro");
        }
        if (strncmp(reader, "PUSH", 4) == 0)
        {
            push(&my_stack, reader + 5);
            send(sock, "Pushed", 6, 0);
        }
        else if (strncmp(reader, "POP", 3) == 0)
        {
            Stack *temp = pop(&my_stack);
            write(sock, (temp != NULL) ? temp->data : "Empty", (temp != NULL) ? sizeof(temp->data) : em);
            if (temp != NULL)
            {
                free(temp->data);
                delete temp;
            }
        }
        else if (strncmp(reader, "TOP", 3) == 0)
        {
            writer = top(my_stack);
            write(sock, (writer != NULL) ? writer : "Empty", (writer != NULL) ? sizeof(writer) : em);
        }
        else if (strncmp(reader, "exit", 4) == 0)
        {
            write(sock, "succ", 4);
            close(sock);
            std::cout << "\nClosing thread and connection" << std::endl;
            break;
        }
        else
        {
            write(sock, "Invaild commands", 16);
        }
    }
    return 0;
}
