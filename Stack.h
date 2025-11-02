// Stack.h

#ifndef STACK_H
#define STACK_H

#include "LinkedList.h"
#include <stdexcept>

// Stack class
template <typename T>
class Stack {
private:
    LinkedList<T> list;

public:
    Stack() {}

    // Push an element onto the stack
    void push(T value) {
        list.insert(0, value); // Insert at the beginning for O(1) time
    }

    // Pop an element from the stack
    T pop() {
        if (isEmpty())
            throw std::out_of_range("Stack Underflow");
        T value = list.get(0);
        list.remove(0);
        return value;
    }

    // Peek the top element
    T top() {
        if (isEmpty())
            throw std::out_of_range("Stack is empty");
        return list.get(0);
    }

    // Check if the stack is empty
    bool isEmpty() {
        return list.getSize() == 0;
    }
};

#endif // STACK_H
