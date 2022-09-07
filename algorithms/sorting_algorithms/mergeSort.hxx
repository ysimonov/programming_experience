#include <iostream>

template <typename T>
void merge(T* arr, int from, int to, int mid) {
    int i = from;
    int j = mid + 1;
    int k = 0;

    T* temp = new T[to - from + 1];

    // merge two parts into temp
    while ((i <= mid) && (j <= to)) {
        if (arr[i] < arr[j]) {
            temp[k] = arr[i];
            k++;
            i++;
        } else {
            temp[k] = arr[j];
            k++;
            j++;
        }
    }

    // insert all the remaining values from i to mid into temp[]
    while (i <= mid) {
        temp[k] = arr[i];
        k++;
        i++;
    }

    // insert all the remaining values from j to to into temp[]
    while (j <= to) {
        temp[k] = arr[j];
        k++;
        j++;
    }

    // assign sorted data stored in temp[] to arr[]
    for (i = from; i <= to; i++) {
        arr[i] = temp[i - from];
    }

    delete[] temp;
}

template <typename T>
void mergeSort(T* arr, int from, int to) {
    int mid;
    if (from < to) {
        mid = (from + to) / 2;
        if (from < mid) {
            mergeSort<T>(arr, from, mid);
        }
        if ((mid + 1) < to) {
            mergeSort<T>(arr, mid + 1, to);
        }
        merge<T>(arr, from, to, mid);
    }
}