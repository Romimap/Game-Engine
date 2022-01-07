#ifndef LINKEDQUEUE_H
#define LINKEDQUEUE_H


template <typename T> struct LinkedQueueNode {
    LinkedQueueNode<T>* _next;
    T* _data;
    LinkedQueueNode(T *data) {
        _next = nullptr;
        _data = data;
    }
};


struct OctreeRendererChange;
template struct LinkedQueueNode<OctreeRendererChange>;


template <typename T> class LinkedQueue {
    /*** ATTRIBUTES ***/
private:
    int _size = 0;

    LinkedQueueNode<T>* _front = nullptr;
    LinkedQueueNode<T>* _back = nullptr;

    /*** METHODS ***/
public:
    LinkedQueue();
    LinkedQueue(T *value);

    void push_back(T *value);
    T* pop_front();
    bool empty();
    int size();
    void push_back_clear(LinkedQueue<T> &queue);
};


#endif // LINKEDQUEUE_H

