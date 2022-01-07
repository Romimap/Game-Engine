#include "linkedqueue.h"

template<typename T> LinkedQueue<T>::LinkedQueue() {

}

template<typename T> LinkedQueue<T>::LinkedQueue(T *value) {
    push_back(value);
}

template<typename T> void LinkedQueue<T>::push_back(T *value) {
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

template<typename T> T* LinkedQueue<T>::pop_front() {
    if (_front == nullptr) return nullptr;
    T* value = _front->_data;
    LinkedQueueNode<T>* tmp = _front;
    _front = _front->_next;
    delete tmp;
    _size--;
    return value;
}

template<typename T> bool LinkedQueue<T>::empty() {
    return _size == 0;
}

template<typename T> int LinkedQueue<T>::size() {
    return _size;
}

///Front (This queue) Back <- Front (&queue) Back
template<typename T> void LinkedQueue<T>::push_back_clear(LinkedQueue<T> &queue) {
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

struct OctreeRendererChange;
template class LinkedQueue<OctreeRendererChange>;
