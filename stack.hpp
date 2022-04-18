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
using namespace std;
pthread_mutex_t lock;
class Stack
{
public:
    char* data;
    Stack *next;
};

Stack *newNode(char* data){
    Stack *stack = new Stack();
    stack->data = data;
    stack->next = NULL;
    return stack;
}

int isEmpty(Stack *root)
{
    return !root;
}

void push(Stack **root, char* data)
{
    pthread_mutex_lock(&lock);
    Stack *Stack = newNode(data);
    Stack->next = *root;
    *root = Stack;
}

char* pop(Stack **root)
{
    if (isEmpty(*root))
        return "";
    Stack *temp = *root;
    *root = (*root)->next;
    char* popped = temp->data;
    free(temp);
    return popped;
}

char* top(Stack *root)
{
    char* s;
    if (isEmpty(root))
        s = "";
    else
        s = root->data;
    return s;
}