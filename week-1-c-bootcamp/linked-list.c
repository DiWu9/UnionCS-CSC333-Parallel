//linked-list.c
//author: John Rieffel
//modified by: Di Wu

#include <stdio.h>
#include <stdlib.h>
// ------ a node has              \
// | ---|--->a pointer-to-node field
// ------   a data field
// | 17 | and it has the type-name: Node
//------
typedef struct node
{
    struct node *next;
    int data;
} Node;

Node *insert(int N, Node *listptr)
{
    //allocate space for new Node

    struct node *tmpptr = (struct node *)malloc(sizeof(Node));
    tmpptr->data = N;
    tmpptr->next = NULL;

    //insert as list head
    if (listptr == NULL || N < listptr->data)
    {
        tmpptr->next = listptr;
        listptr = tmpptr;
    }
    //insert mid or insert end
    else
    {
        struct node *curptr = listptr;
        //exit when ptr is null or ptr+1->data > N
        while (curptr->next != NULL && curptr->next->data <= N)
        {
            curptr = curptr->next;
        }
        tmpptr->next = curptr->next;
        curptr->next = tmpptr;
    }
    return listptr;
}

Node *insertRecursive(int N, Node *listptr)
{
    struct node *tmpptr = (struct node *)malloc(sizeof(Node));
    tmpptr->data = N;
    tmpptr->next = NULL;
    // base1: empty list, insert head
    if (listptr == NULL || N < listptr->data)
    {
        tmpptr->next = listptr;
        listptr = tmpptr;
    }
    // base2: insert after head
    else if (listptr->data <= N && (listptr->next == NULL || listptr->next->data > N))
    {
        tmpptr->next = listptr->next;
        listptr->next = tmpptr;
    }
    else
    {
        insertRecursive(N, listptr->next);
    }
    return listptr;
}

void PrintList(Node *listptr)
{
    if (listptr == NULL)
    {
        printf("NULL");
    }
    else
    {
        printf("%d->", listptr->data);
        PrintList(listptr->next);
    }
    return;
}

void PrintListByLoop(Node *listptr)
{
    struct node *curptr = listptr;
    while (curptr != NULL)
    {
        printf("%d->", curptr->data);
        curptr = curptr->next;
    }
    printf("NULL");
    return;
}

int main()
{
    struct node *list = NULL;
    list = insertRecursive(10, list);
    list = insertRecursive(5, list);
    list = insertRecursive(7, list);
    list = insertRecursive(12, list);
    list = insertRecursive(1, list);
    list = insertRecursive(14, list);
    list = insertRecursive(-4, list);
    list = insertRecursive(-18, list);
    printf("%d\n", list->data);
    PrintList(list);
}