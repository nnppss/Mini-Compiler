// LinkedList.h

#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <iostream>
#include <stdexcept>

// Node structure for the linked list
template <typename T>
class ListNode {
public:
    T data;
    ListNode* next;

    ListNode(T value) : data(value), next(nullptr) {}
};

// Linked List class
template <typename T>
class LinkedList {
private:
    ListNode<T>* head;
    int size;

public:
    LinkedList() : head(nullptr), size(0) {}

    // Function to add a node at the end
    void append(T value) {
        ListNode<T>* newNode = new ListNode<T>(value);
        if (!head) {
            head = newNode;
        } else {
            ListNode<T>* temp = head;
            while (temp->next)
                temp = temp->next;
            temp->next = newNode;
        }
        size++;
    }

    // Function to insert a node at a specific index
    void insert(int index, T value) {
        if (index < 0 || index > size)
            throw std::out_of_range("Index out of range");
        ListNode<T>* newNode = new ListNode<T>(value);
        if (index == 0) {
            newNode->next = head;
            head = newNode;
        } else {
            ListNode<T>* temp = head;
            for (int i = 0; i < index - 1; i++)
                temp = temp->next;
            newNode->next = temp->next;
            temp->next = newNode;
        }
        size++;
    }

    // Function to remove a node at a specific index
    void remove(int index) {
        if (index < 0 || index >= size)
            throw std::out_of_range("Index out of range");
        ListNode<T>* temp = head;
        if (index == 0) {
            head = head->next;
            delete temp;
        } else {
            for (int i = 0; i < index - 1; i++)
                temp = temp->next;
            ListNode<T>* nodeToDelete = temp->next;
            temp->next = nodeToDelete->next;
            delete nodeToDelete;
        }
        size--;
    }

    // Function to get the value at a specific index
    T get(int index) {
        if (index < 0 || index >= size)
            throw std::out_of_range("Index out of range");
        ListNode<T>* temp = head;
        for (int i = 0; i < index; i++)
            temp = temp->next;
        return temp->data;
    }

    // Function to set the value at a specific index
    void set(int index, T value) {
        if (index < 0 || index >= size)
            throw std::out_of_range("Index out of range");
        ListNode<T>* temp = head;
        for (int i = 0; i < index; i++)
            temp = temp->next;
        temp->data = value;
    }

    // Function to get the size of the list
    int getSize() {
        return size;
    }

    // Destructor to free memory
    ~LinkedList() {
        ListNode<T>* temp = head;
        while (temp) {
            ListNode<T>* next = temp->next;
            delete temp;
            temp = next;
        }
    }
};

#endif // LINKEDLIST_H
