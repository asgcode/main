#ifndef STACK
#define STACK

#include<stdlib.h>

class Node
{
public:
    Node* next;
    char* string;

    Node(char* str):next(NULL),string(str)
    {
    }
};

class Stack
{

    Node* head;

public:

    Stack():head(NULL)
    {

    }

    void push(Node* pNode)
    {
        if (head == NULL)
        {
            head = pNode;
        }
        else
        {
            Node* oldHead = head;
            head = pNode;
            head->next = oldHead;
        }
    }

    Node* pop()
    {
        Node* currentHead = head;
        head = head->next;
        return currentHead;
    }


};

#endif //STACK
