#include <stdbool.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string>

using namespace std;

struct Node{
    struct Node *next;
    struct Node *prev;
    string buff;
};

struct Stack{
    struct Node *p;
    bool flag = false;
};

string pop(struct Stack*);
bool push(struct Stack*, string);
string top(struct Stack);