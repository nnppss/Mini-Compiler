/*
#ifndef QUEUE_H
#define QUEUE_H

#include <queue>

template <typename T>
class Queue {
private:
    std::queue<T> data;

public:
    void enqueue(const T& value) {
        data.push(value);
    }

    T dequeue() {
        T value = data.front();
        data.pop();
        return value;
    }

    T front() const {
        return data.front();
    }

    bool isEmpty() const {
        return data.empty();
    }

    size_t getSize() const {
        return data.size();
    }
};

#endif // QUEUE_H
*/

#ifndef QUEUE_H
#define QUEUE_H

#include <stdexcept> // For std::out_of_range

template <typename T>
class Queue {
private:
    // Node structure for linked list
    struct Node {
        T data;
        Node* next;
        Node(const T& value) : data(value), next(nullptr) {}
    };

    Node* frontNode; // Pointer to the front node
    Node* rearNode;  // Pointer to the rear node
    size_t size;     // Number of elements in the queue

public:
    Queue() : frontNode(nullptr), rearNode(nullptr), size(0) {}

    ~Queue() {
        while (frontNode != nullptr) {
            Node* temp = frontNode;
            frontNode = frontNode->next;
            delete temp;
        }
    }

    void enqueue(const T& value) {
        Node* newNode = new Node(value);
        if (rearNode == nullptr) {
            // If the queue is empty
            frontNode = rearNode = newNode;
        } else {
            rearNode->next = newNode;
            rearNode = newNode;
        }
        ++size;
    }

    T dequeue() {
        if (isEmpty()) {
            throw std::out_of_range("Queue is empty");
        }
        T value = frontNode->data;
        Node* temp = frontNode;
        frontNode = frontNode->next;
        delete temp;
        if (frontNode == nullptr) {
            // If the queue is now empty
            rearNode = nullptr;
        }
        --size;
        return value;
    }

    T front() const {
        if (isEmpty()) {
            throw std::out_of_range("Queue is empty");
        }
        return frontNode->data;
    }

    bool isEmpty() const {
        return frontNode == nullptr;
    }

    size_t getSize() const {
        return size;
    }
};

#endif // QUEUE_H
