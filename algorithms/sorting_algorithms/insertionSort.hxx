#include <iostream>

#include "common.hpp"

typedef long long int ssize_t;

template <typename T>
void insertionSort(T arr[], int n) {
    for (size_t i = 1; i < n; i++) {
        ssize_t j = ssize_t(i);
        while (j > 0) {
            if (arr[j - 1] <= arr[j]) {
                break;
            }
            swap<T>(&arr[j], &arr[j - 1]);
            j -= 1;
        }
    }
}
