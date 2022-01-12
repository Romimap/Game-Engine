/*
    LINKEDQUEUE.H
    Queue implemented as a linked list. Used so the Push, Pop & Merge are O(1)
*/

#ifndef LINKEDQUEUE_H
#define LINKEDQUEUE_H


template <typename T> struct LinkedQueueNode {
    LinkedQueueNode<T>* _next;
    T _data;
    LinkedQueueNode(T data) {
        _next = nullptr;
        _data = data;
    }
};


template <typename T> class LinkedQueue {
    /*** ATTRIBUTES ***/
private:
    int _size = 0;

    LinkedQueueNode<T>* _front = nullptr;
    LinkedQueueNode<T>* _back = nullptr;

    /*** METHODS ***/
public:
    LinkedQueue(){}

    LinkedQueue(T value) {
        push_back(value);
    }

    ///Pushes the element <value> on the end of the queue
    void push_back(T value) {
        LinkedQueueNode<T>* node = new LinkedQueueNode<T>(value);
        if (empty()) {
            _front = node;
            _back = node;
        } else {
            _back->_next = node;
            _back = node;
        }
        _size++;
    }

    ///Pops the front of the queue
    T pop_front() {
        if (_front == nullptr) return nullptr;
        T value = _front->_data;
        LinkedQueueNode<T>* tmp = _front;
        _front = _front->_next;
        delete tmp;
        _size--;
        return value;
    }

    ///Returns true if the queue is empty
    bool empty() {
        return _size == 0;
    }

    ///Returns the number of elements currently waiting in the queu
    int size() {
        return _size;
    }

    ///Empties <queue> into this queue
    void push_back_clear(LinkedQueue<T> &queue) {
        if (queue.empty()) return;

        if (empty()) {
            _back = queue._back;
            _front = queue._front;
            _size = queue._size;
        } else {
            _back->_next = *&queue._front;
            _back = *&queue._back;
            _size += queue._size;
        }

        queue._back = nullptr;
        queue._front = nullptr;
        queue._size = 0;
    }
};

#endif // LINKEDQUEUE_H
