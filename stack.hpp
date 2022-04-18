#include <stdbool.h>

struct Node{
    struct Node *next;
    struct Node *prev;
    char buff[1024];
};

struct Stack{
    struct Node *p;
    bool flag = false;
};

bool pop(struct Stack*);
bool push(struct Stack*, char*);
char *top(struct Stack);