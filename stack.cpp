#include "stack.hpp"

/**
 * @brief simple pop implemitation of stack
 * @param my_stack 
 * @return true if succed to pop
 */
string pop(struct Stack *my_stack){
    if(my_stack->flag){
        string er = "The stack is empty";
        return er;
    }

    struct Node *curr = my_stack->p;
    // if there are only one node in stack
    if(curr->prev == NULL){
        my_stack->p = NULL;
        my_stack->flag = true;
        return curr->buff;
    }

    curr->prev->next = NULL;
    my_stack->p = my_stack->p->prev;
}

bool push(struct Stack *my_stack, string input){
    struct Node *temp = (struct Node*)malloc(sizeof(struct Node));
    // copy the given input to the temp node
    temp->buff = input;

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

string top(struct Stack my_stack){
    if(my_stack.p){
        return my_stack.p->buff;
    }
    else{
        string empty = "nothing to show..";
        return empty;
    }
}