#ifndef LINKEDQUEUE_H
#define LINKEDQUEUE_H

template <typename T> struct LinkedQueueNode {
    LinkedQueueNode<T>* _next;
    T* _data;
    LinkedQueueNode(T &data) {
        _next = nullptr;
        _data = &data;
    }
};

struct A;
template struct LinkedQueueNode<A>;

template <typename T> class LinkedQueue {
private:
    LinkedQueueNode<T>* _front = nullptr;
    LinkedQueueNode<T>* _back = nullptr;
    int _size = 0;
public:
    LinkedQueue();
    LinkedQueue(T &value);
    void push_back(T &value);
    T* pop_front();
    bool empty();
    int size();
    void push_back_clear(LinkedQueue<T> &queue);
};


#endif // LINKEDQUEUE_H

