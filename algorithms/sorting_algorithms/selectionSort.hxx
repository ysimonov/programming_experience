#include "common.hpp"

#include <iostream>

template<typename T>
void selectionSort(T arr[], int n) {
    for (size_t k = 0; k < n-1; k++) {
        auto idx_min = k; // index of the smallest element
        for (size_t j = k+1; j < n; j++) {
            if (arr[j] < arr[idx_min]) {
                idx_min = j;
            }
        }
        swap(&arr[idx_min], &arr[k]);
    }
}