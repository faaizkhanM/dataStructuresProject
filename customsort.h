#ifndef CUSTOMSORT_H
#define CUSTOMSORT_H

#include "customvector.h"

namespace CustomSort {

// Bubble Sort implementation
template<typename T, typename Compare>
void bubbleSort(CustomVector<T>& arr, Compare comp) {
    size_t n = arr.size();
    for (size_t i = 0; i < n - 1; i++) {
        for (size_t j = 0; j < n - i - 1; j++) {
            if (comp(arr[j + 1], arr[j])) {
                // Swap elements
                T temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

// Merge Sort implementation
template<typename T, typename Compare>
void mergeSort(CustomVector<T>& arr, Compare comp) {
    if (arr.size() <= 1) return;
    
    // Helper function for merge
    auto merge = [&](size_t left, size_t mid, size_t right) {
        CustomVector<T> temp(right - left + 1);
        size_t i = left, j = mid + 1, k = 0;
        
        while (i <= mid && j <= right) {
            if (comp(arr[i], arr[j])) {
                temp[k++] = arr[i++];
            } else {
                temp[k++] = arr[j++];
            }
        }
        
        while (i <= mid) {
            temp[k++] = arr[i++];
        }
        
        while (j <= right) {
            temp[k++] = arr[j++];
        }
        
        for (size_t i = 0; i < k; i++) {
            arr[left + i] = temp[i];
        }
    };
    
    // Helper function for recursive merge sort
    auto mergeSortHelper = [&](size_t left, size_t right) {
        if (left < right) {
            size_t mid = left + (right - left) / 2;
            mergeSortHelper(left, mid);
            mergeSortHelper(mid + 1, right);
            merge(left, mid, right);
        }
    };
    
    mergeSortHelper(0, arr.size() - 1);
}

// Selection Sort implementation
template<typename T, typename Compare>
void selectionSort(CustomVector<T>& arr, Compare comp) {
    size_t n = arr.size();
    for (size_t i = 0; i < n - 1; i++) {
        size_t min_idx = i;
        for (size_t j = i + 1; j < n; j++) {
            if (comp(arr[j], arr[min_idx])) {
                min_idx = j;
            }
        }
        if (min_idx != i) {
            // Swap elements
            T temp = arr[i];
            arr[i] = arr[min_idx];
            arr[min_idx] = temp;
        }
    }
}

// Insertion Sort implementation
template<typename T, typename Compare>
void insertionSort(CustomVector<T>& arr, Compare comp) {
    size_t n = arr.size();
    for (size_t i = 1; i < n; i++) {
        T key = arr[i];
        size_t j = i;
        
        while (j > 0 && comp(key, arr[j - 1])) {
            arr[j] = arr[j - 1];
            j--;
        }
        arr[j] = key;
    }
}

} // namespace CustomSort

#endif // CUSTOMSORT_H
