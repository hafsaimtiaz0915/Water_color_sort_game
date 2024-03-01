#include <iostream>
#include "stack.h"
using namespace std;

class Node
{
public:
    Stack<int> stacks[14];
    Node *next;
    Node *prev;
    int tubes_no;
    int leftMove;

    Node()
    {
        for (int i = 0; i < 14; ++i)
        {
            stacks[i].set_size(4);
        }

        next = nullptr;
        prev = nullptr;
    }
    Node(Stack<int> tubes[], int tubesNo, int leftmove)
    {
        tubes_no = tubesNo;
        leftMove = leftmove;
        for (int i = 0; i < tubesNo; ++i)
        {
            stacks[i].set_size(4);
        }
        next = nullptr;
        prev = nullptr;
        for (int i = 0; i < tubesNo - 2; ++i)
        {
            for (int j = 0; j < 4; j++)
            {
                stacks[i].push(tubes[i].get_index(j));
            }
        }
    }
    ~Node()
    {
    }
};

class DoublyLinkedList
{
public:
    Node *head;
    Node *tail;

    DoublyLinkedList()
    {
        head = nullptr;
        tail = nullptr;
    }

    void appendNode(Stack<int> tubes[], int tubesNo, int leftmove)
    {
        Node *newNode = new Node(tubes, tubesNo, leftmove);

        if (head == nullptr)
        {
            head = newNode;
            tail = newNode;
        }
        else
        {
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        }
    }

    void display()
    {
        Node *temp = head;
        int nodeIndex = 1;

        while (temp != nullptr)
        {
            for (int i = 0; i < temp->tubes_no; ++i)
            {
                cout << "Node " << nodeIndex << ", Stack " << i + 1 << ": ";
                temp->stacks[i].display();
            }
            temp = temp->next;
            ++nodeIndex;
        }
        cout << "NULL" << endl;
    }

    Node *getNode(int nodeIndex)
    {
        Node *temp = head;
        int currentIndex = 1;

        while (temp != nullptr && currentIndex < nodeIndex)
        {
            temp = temp->next;
            ++currentIndex;
        }

        return temp;
    }

    ~DoublyLinkedList()
    {
        Node *temp = head;
        while (temp != nullptr)
        {
            Node *nextNode = temp->next;
            delete temp;
            temp = nextNode;
        }
    }
};