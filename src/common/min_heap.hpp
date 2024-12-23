#ifndef __PRIORITY_QUEUE_HPP__
#define __PRIORITY_QUEUE_HPP__

// Custom implementation of Minimum Heap

#include <stdlib.h>
#include <stdio.h>
#include "custom_utils.hpp"
// Reference 1: https://www.programiz.com/dsa/priority-queue
// Reference 2: https://perplexity.ai
// (asked about missed points from the ref.1 regarding min-heap
// + was advised to move template stuff into separate *.tpp files)

// few constants
enum { min_capacity = 8 };
enum { capacity_multiplier = 2 };


// queue (heap) element abstraction
template <class T, class I>
struct QueueElement
{
    T *data; // associated data chunk
    I value; // priority value

    QueueElement(T *data_ptr = 0, I val = 0)
        : data(data_ptr)
        , value(val)
        {};

    bool operator==(const QueueElement& other) const
        { return this->value == other->value; }

    bool operator>=(const QueueElement& other) const
        { return this->value >= other->value; }

    bool operator<=(const QueueElement& other) const
        { return this->value <= other->value; }

    bool operator>(const QueueElement& other) const
        { return this->value > other->value; }

    bool operator<(const QueueElement& other) const
        { return this->value < other->value; }
};

// Headers of the Min-Heap helper functions (templates)
template <class T, class I, class U>
class MinHeap;

template <class T, class I, class U>
void resize(MinHeap<T, I, U>& pq, U new_capacity);

template <class T, class I, class U>
void heapify_up(MinHeap<T, I, U>& pq, U i);

template <class T, class I, class U>
void heapify_down(MinHeap<T, I, U>& pq, U i);

template <class T, class I, class U>
void insert(MinHeap<T, I, U>& pq, QueueElement<T, I>& new_element);

template <class T, class I, class U>
QueueElement<T, I> *remove_min_value(MinHeap<T, I, U>& pq);


// MinHeap class abstraction
template <class T, class I, class U>
class MinHeap {
    QueueElement<T, I> *queue;
    U size;
    U capacity;
public:
    MinHeap()
        : size(0)
        , capacity(min_capacity)
        { queue = new QueueElement<T, I>[capacity](); }

    ~MinHeap()
        { delete[] queue; }

    QueueElement<T, I> *pop()
        { return remove_min_value(*this); }
    void push(QueueElement<T, I> *element)
        { insert(*this, *element); }
    
    U get_size() const
        { return size; }
private:
    MinHeap(const MinHeap<T, I, U>& pq) {} // copy prohibited
    I& operator[](U ind);

    friend void resize<>(MinHeap<T, I, U>& pq, U new_capacity);
    friend void heapify_up<>(MinHeap<T, I, U>& pq, U i);
    friend void heapify_down<>(MinHeap<T, I, U>& pq, U i);
    friend void insert<>(
        MinHeap<T, I, U>& pq, QueueElement<T, I>& element
    );
    friend QueueElement<T, I>* remove_min_value<>(
        MinHeap<T, I, U>& pq
    );
};

// enable automatic instantiation
// by including the corresponding templates explicitly
#include "min_heap.tpp"

#endif

// Author: Pavel Kharyuk, 2024