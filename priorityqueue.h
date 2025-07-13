#ifndef PRIORITYQUEUE_H
#define PRIORITYQUEUE_H

#include "customvector.h"
#include "cab.h"

class PriorityQueue {
private:
    CustomVector<Cab> heap;

    void heapifyUp(int index) {
        while (index > 0) {
            int parent = (index - 1) / 2;
            if (heap[parent].getPriority() < heap[index].getPriority()) {
                std::swap(heap[parent], heap[index]);
                index = parent;
            } else {
                break;
            }
        }
    }

    void heapifyDown(int index) {
        int size = heap.size();
        while (true) {
            int leftChild = 2 * index + 1;
            int rightChild = 2 * index + 2;
            int largest = index;

            if (leftChild < size && heap[leftChild].getPriority() > heap[largest].getPriority()) {
                largest = leftChild;
            }
            if (rightChild < size && heap[rightChild].getPriority() > heap[largest].getPriority()) {
                largest = rightChild;
            }

            if (largest != index) {
                std::swap(heap[index], heap[largest]);
                index = largest;
            } else {
                break;
            }
        }
    }

public:
    void push(const Cab &cab) {
        heap.push_back(cab);
        heapifyUp(heap.size() - 1);
    }

    Cab top() const {
        if (heap.empty()) {
            throw std::runtime_error("Priority queue is empty");
        }
        return heap[0];
    }

    void pop() {
        if (heap.empty()) {
            throw std::runtime_error("Priority queue is empty");
        }
        heap[0] = heap[heap.size() - 1];
        heap.pop_back();
        if (!heap.empty()) {
            heapifyDown(0);
        }
    }

    bool empty() const {
        return heap.empty();
    }
};

#endif // PRIORITYQUEUE_H
