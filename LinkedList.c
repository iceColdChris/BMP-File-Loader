#include "LinkedList.h"


void push(listNode * head, char* val)
{
    listNode * current = head;
    
    if(head->val == NULL)
    {
        head->val = malloc(sizeof(char*));
        head->val = val;
    }
    else
    {
        while (current->next != NULL)
        {
            current = current->next;
        }
        
        /* now we can add a new variable */
        current->next = malloc(sizeof(listNode));
        current->next->val = malloc(sizeof(char*));
        current->next->val = val;
    }
}
