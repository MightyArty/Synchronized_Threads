#include "client.hpp"
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
void sig_handler(int signum)
{
    switch (signum)
    {
    case SIGTSTP:
        red();
        printf("\nI'm the first signal..\n");
    case SIGINT:
        yellow();
        printf("\nI'm the second signal, trying to divide\n");
    default:
        reset();
        close(listenFd);
        exit(1);
    }
}
int client()
{
    int portNo;
    bool loop = false;
    struct sockaddr_in svrAdd;
    struct hostent *server;

    // if (argc < 2)
    // {
    //     cerr << "Syntam : ./server <port>" << endl;
    //     return 0;
    // }

    // portNo = atoi(argv[1]);
    portNo = htons(3006);

    if ((portNo > 65535) || (portNo < 2000))
    {
        cerr << "Please enter a port number between 2000 - 65535" << endl;
        return 0;
    }

    listenFd = socket(AF_INET, SOCK_STREAM, 0);

    if (listenFd < 0)
    {
        cerr << "Cannot open socket" << endl;
        return 0;
    }

    server = gethostbyname("localhost");

    if (server == NULL)
    {
        cerr << "Host does not exist" << endl;
        return 0;
    }

    bzero((char *)&svrAdd, sizeof(svrAdd));
    svrAdd.sin_family = AF_INET;

    bcopy((char *)server->h_addr, (char *)&svrAdd.sin_addr.s_addr, (size_t)server->h_length);

    svrAdd.sin_port = htons(portNo);

    int checker = connect(listenFd, (struct sockaddr *)&svrAdd, sizeof(svrAdd));

    if (checker < 0)
    {
        cerr << "Cannot connect!" << endl;
        return 0;
    }
    return 1;
}
int main(int argc, char *argv[])
{

    signal(SIGINT, sig_handler);
    signal(SIGTSTP, sig_handler);
    if (!client())
        return 0;
    for (;;)
    {
        cout << "Enter stuff: ";
        bzero(w, BUFFSIZE);
        cin.getline(w, BUFFSIZE);
        write(listenFd, w, strlen(w));
        bzero(r, BUFFSIZE);
        read(listenFd, r, BUFFSIZE);
        puts(r);
        string exit(w);
        if (exit == "exit")
        {
            close(listenFd);
            return 1;
        }
    }
}