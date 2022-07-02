#include "common.hpp"

#include <iostream>

template<typename T>
void quickSort(T* arr, int left, int right) {
    int l = left;
    int r = right - 1;
    int size = right - left;
    if (size > 1) { 
        T pivot = arr[rand() % size + l];
        while (l < r) {
            while (arr[r] > pivot && r > l) {
                r--;
            }
            while (arr[l] < pivot && l <= r) {
                l++;
            }
            if (l < r) {
                swap<T>(&arr[l], &arr[r]);
                l++;
            }
        }
        quickSort<T>(arr, left, l);
        quickSort<T>(arr, r, right);
    }
}
