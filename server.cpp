#include "server.hpp"
#include "malloc.h"
#define em 5
/**
 * @brief The functions welcome,red,yellow,blue,green and reset are just for fun
 * We want you to enjoy the proccess :)
 */
void welcome()
{
    printf("\033[1;31m    $$      $$$$$  $$$$$$$$$ $     $         \033[1;34m $$$$$         $$     $$$$$       $$      $  $    \n");
    printf("\033[1;31m   $  $     $   $      $      $   $          \033[1;34m $   $        $  $    $   $      $  $     $ $    \n");
    printf("\033[1;31m  $ -- $    $$$$$      $        $    \033[1;33m @@@@@@ \033[1;34m $$$$$$$     $ -- $   $$$$$     $ -- $    $$        \n");
    printf("\033[1;31m $      $   $    $     $        $            \033[1;34m $     $    $      $  $    $   $      $   $ $         \n");
    printf("\033[1;31m$        $  $     $    $        $            \033[1;34m $$$$$$$   $        $ $     $ $        $  $  $       \n");
}
void red()
{
    printf("\033[1;31m");
}
void yellow()
{
    printf("\033[1;33m");
}
void blue()
{
    printf("\033[0;34m");
}
void green()
{
    printf("\033[0;32m");
}
void reset()
{
    printf("\033[0m");
}
void free_stack(Stack **root)
{
    pthread_mutex_lock(&lock);
    while (*root)
    {
        Stack *temp = *root;
        *root = (*root)->next;
        _free(temp->data);
        delete temp;
    }

    *root = NULL;
    size = 0;
    pthread_mutex_unlock(&lock);

    std::cout << "free all allocate" << std::endl;
}
void sig_handler(int signum)
{
    free_stack(&my_stack);
    switch (signum)
    {
    case SIGTSTP:
        red();
        puts("");
        printf("Trying to exit on CONTROL-Z command\n");
    case SIGINT:
        yellow();
        printf("Trying to exit on CONTROL-C command\n");
    case SIGQUIT:
        green();
        printf("Trying to exit on CONTROL-/ command\n");
    default:
        close(listenFd);
        std::cout << "Closing Server" << std::endl;
    }
}
Stack *newNode(char *data)
{
    Stack *stack = new Stack();
    stack->data = strcpy((char *)_malloc(BUFFSIZE), data);
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
int server(int argc, char *argv[])
{

    if (argc >= 2)
    {
        try
        {
            portNo = atoi(argv[1]);
            if ((portNo > 65535) || (portNo < 2000))
            {
                throw std::invalid_argument("Please enter a port number between 2000 - 65535");
            }
        }
        catch (const std::exception &e)
        {
            std::cerr << e.what() << '\n';
            portNo = htons(3003);
            std::cout << "Port :" << portNo << std::endl;
        }
    }
    else
    {
        portNo = htons(3003);
        std::cout << "Port :" << portNo << std::endl;
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
    red();
    welcome();
    reset();
    signal(SIGINT, sig_handler);
    signal(SIGTSTP, sig_handler);
    signal(SIGQUIT, sig_handler);
    if (!server(argc, argv))
        return 0;
    while (noThread < 100)
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

    for (int i = 0; i < 100; i++)
    {
        pthread_join(threadA[i], NULL);
    }
    if (size != 0)
        free_stack(&my_stack);
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
            puts("error");
        }
        if (strncmp(reader, "PUSH", 4) == 0)
        {
            puts("Pushed");
            push(&my_stack, reader + 5);
            send(sock, "Pushed", 6, 0);
        }
        else if (strncmp(reader, "POP", 3) == 0)
        {
            Stack *temp = pop(&my_stack);
            write(sock, (temp != NULL) ? temp->data : "Empty", (temp != NULL) ? sizeof(temp->data) : em);
            if (temp != NULL)
            {
                _free(temp->data);
                delete temp;
            }
        }
        else if (strncmp(reader, "TOP", 3) == 0)
        {
            writer = top(my_stack);
            write(sock, (writer != NULL) ? writer : "Empty", (writer != NULL) ? sizeof(writer) : em);
        }
        else if (strncmp(reader, "COUNT", 5) == 0)
        {
            int number = size;
            char numberArray[10] = {0};
            if (size != 0)
            {
                for (int n = log10(size) + 1, i = n - 1; i >= 0; --i, number /= 10)
                {
                    numberArray[i] = (number % 10) + '0';
                }
                write(sock, numberArray, 10);
            }
            else
            {
                write(sock, "0", 1);
            }
        }
        else if (strncmp(reader, "CLEAN", 5) == 0)
        {
            if (size != 0)
            {
                free_stack(&my_stack);
            }
            write(sock, "Clean stack succeeded", 21);
        }
        else if (strncmp(reader, "EXIT", 4) == 0)
        {
            write(sock, "succ", 4);
            close(sock);
            std::cout << "\nClosing thread and connection" << std::endl;
            break;
        }
        else
        {
            write(sock, "(-1)", 4);
        }
    }
    return 0;
}
