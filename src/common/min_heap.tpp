//#include "min_heap.hpp"

template <class T, class I, class U>
I& MinHeap<T, I, U>::operator[](U ind)
{
    if (ind >= this->size) {
        fprintf(
            stderr,
            "MinHeap : index exceeds current size\n"
        );
        exit(EXIT_FAILURE);
    }
    return this->queue[ind].value;
}

template <class U>
U get_parent_ind(U ind)
{
    if (ind == 0)
        return ind;
    return (ind - 1) / 2;
}

template <class U>
U get_lchild_ind(U ind, U size)
{
    U lchild_ind = 2*ind + 1;
    if (lchild_ind < ind || lchild_ind >= size)
        return ind;
    return lchild_ind;
}

template <class U>
U get_rchild_ind(U ind, U size)
{
    U rchild_ind = 2*ind + 2;
    if (rchild_ind < ind || rchild_ind >= size)
        return ind;
    return rchild_ind;
}

template <class T, class I, class U>
void resize(MinHeap<T, I, U>& pq, U new_capacity)
{
    if (new_capacity < pq.size) {
        fprintf(
            stderr,
            "resize : new capacity is too small\n"
        );
        exit(EXIT_FAILURE);
    }
    QueueElement<T, I> *new_queue = (
        new QueueElement<T, I>[new_capacity]()
    );
    for (U i = 0; i < pq.size; i++)
        new_queue[i] = pq.queue[i];
    delete [] pq.queue;
    pq.queue = new_queue;
    pq.capacity = new_capacity;
}

template <class T, class I, class U>
static void swap(QueueElement<T, I> *a, U i, U j)
{
    QueueElement<T, I> t = a[i];
    a[i] = a[j];
    a[j] = t;
}

template <class T, class I, class U>
void heapify_up(MinHeap<T, I, U>& pq, U ind)
{
    if (ind == 0)
        return;
    U parent_ind = get_parent_ind(ind);
    if (pq[parent_ind] <= pq[ind])
        return;
    swap(pq.queue, parent_ind, ind);
    heapify_up(pq, parent_ind);
}

template <class T, class I, class U>
void heapify_down(MinHeap<T, I, U>& pq, U ind)
{
    U min_ind = ind;

    U lchild_ind = get_lchild_ind(ind, pq.size);
    if (pq[lchild_ind] < pq[min_ind])
        min_ind = lchild_ind;

    U rchild_ind = get_rchild_ind(ind, pq.size);
    if (pq[rchild_ind] < pq[min_ind])
        min_ind = rchild_ind;

    if (min_ind == ind)
        return;

    swap(pq.queue, ind, min_ind);
    heapify_down(pq, min_ind);
}

template <class T, class I, class U>
void insert(MinHeap<T, I, U>& pq, QueueElement<T, I>& new_element)
{
    if (capacity_multiplier*pq.size >= pq.capacity)
        resize<T, I, U>(pq, pq.capacity*capacity_multiplier);
    pq.queue[pq.size] = new_element;
    pq.size += 1;
    heapify_up(pq, pq.size-1);
}

template <class T, class I, class U>
QueueElement<T, I> *remove_min_value(MinHeap<T, I, U>& pq)
{
    U root_ind = 0;
    if (pq.size == 0)
        return 0;
    QueueElement<T, I> *rv = new QueueElement<T, I>(pq.queue[root_ind]);
    if (pq.size == 1) {
        pq.queue[root_ind] = QueueElement<T, I>();
        pq.size = 0;
        return rv;
    }
    pq.queue[root_ind] = pq.queue[pq.size-1];
    pq.queue[pq.size-1] = QueueElement<T, I>();
    pq.size -= 1;
    heapify_down(pq, root_ind);

    if (
        (pq.capacity > min_capacity)
        && (capacity_multiplier*pq.size <= pq.capacity)
    )
        resize<T, I, U>(pq, pq.capacity/capacity_multiplier);
    return rv;
}

// Author: Pavel Kharyuk, 2024