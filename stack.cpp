#include "stack.hpp"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/**
 * @brief simple pop implemitation of stack
 * @param my_stack 
 * @return true if succed to pop
 */
bool pop(struct Stack *my_stack){
    if(my_stack->flag){
        printf("The stack is empty..\n");
        return false;
    }

    struct Node *curr = my_stack->p;
    // if there are only one node in stack
    if(curr->prev == NULL){
        my_stack->p = NULL;
        my_stack->flag = true;
        return true;
    }
    curr->prev->next = NULL;
    my_stack->p = my_stack->p->prev;
}

bool push(struct Stack *my_stack, char *input){
    struct Node *temp = (struct Node*)malloc(sizeof(struct Node));
    // copy the given input to the temp node
    strcpy(temp->buff, input);
    // if the stack is empty
    if(my_stack->flag){
        my_stack->p = temp;
        my_stack->flag = false;
    }
    // if the stack is not empty
    else{
        temp->prev = my_stack->p;
        my_stack->p = temp;
    }
    return true;
}

char *top(struct Stack my_stack){
    if(my_stack.p){
        return my_stack.p->buff;
    }
    else{
        char *empty = "nothing to show..";
        return empty;
    }
}