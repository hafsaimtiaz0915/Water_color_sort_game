#include <iostream>
using namespace std;
template <class T>
class Stack
{
public:
    T *arr;
    int size;
    int capacity;
    int top;

public:
    Stack()
    {
        arr = nullptr;
        size = 0;
        capacity = 0;
        top = -1;
    }
    Stack<T> &operator=(const Stack<T> &other)
    {
        if (this != &other) // Avoid self-assignment
        {
            delete[] arr; // Release existing resources

            capacity = other.capacity;
            top = other.top;

            // Allocate new memory and copy the elements
            arr = new T[capacity];
            for (int i = 0; i <= top; ++i)
            {
                arr[i] = other.arr[i];
            }
        }
        return *this;
    }

    Stack(int size)
    {
        this->size = size;
        arr = new T[size];
        top = -1;
    }
    void set_size(int size)
    {
        this->size = size;
        arr = new T[size];
    }

    bool IsFull()
    {
        return top == size - 1;
    }

    bool IsEmpty()
    {
        return top == -1;
    }

    void push(T data)
    {
        if (!IsFull())
        {
            arr[++top] = data;
            capacity++;
        }
    }
    T get_index(int x)
    {
        return arr[x];
    }

    void pop()
    {
        if (!IsEmpty())
        {
            top--;
            capacity--;
        }
        else
        {
            cout << "Stack is emmpty" << endl;
        }
    }
    T get_top()
    {
        return arr[top];
    }
    int get_size()
    {
        return size;
    }
    int get_capacity()
    {
        return capacity;
    }

    void resize()
    {
        int newSize = size * 2;
        T *newArr = new T[newSize];
        for (int i = 0; i <= top; ++i)
        {
            newArr[i] = arr[i];
        }
        delete[] arr;
        arr = newArr;
        size = newSize;
        cout << "Stack resized. New size: " << size << endl;
    }
    void display()
    {
        if (!IsEmpty())
        {
            for (int i = 0; i < size; i++)
            {
                cout << "value at index " << i << " : " << arr[i] << endl;
            }
        }
        else
        {
            cout << "Your stack is empty" << endl;
        }
    }

    ~Stack()
    {
        delete[] arr;
    }
};