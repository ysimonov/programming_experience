#include <thread>
#include <chrono>

#include "sort.hpp"

#include <iostream>

// compile: g++ -O2 main.cpp -o main

template<typename T>
void printArray(T arr[], int n) {
    for (size_t i = 0; i < n; i++) {
        std::cout << arr[i] << " ";
    }
    std::cout << std::endl;
}

int main() {

    int n;
    int selection;

    std::cout << "0 - bubble sort" << std::endl;
    std::cout << "1 - insertion sort" << std::endl;
    std::cout << "2 - merge sort" << std::endl;
    std::cout << "3 - quick sort" << std::endl;
    std::cout << "4 - selection sort" << std::endl;
    std::cout << "Enter selection: ";
    std::cin >> selection;

    if ((selection < 0) || (selection > 4)) {
        std::cout << "Invalid selection";
        exit(1);
    } 

    std::cout << "Enter size of the array: ";
    std::cin >> n;

    double* arr = new double[n];
    for (size_t i = 0; i < n; i++) {
        arr[i] = rand() % 100; // generate a random number between 0 and 99
    }

    std::cout << "Before Sorting: " << std::endl;
    printArray<double>(arr, n);

    std::cout << "Selection: " << selection << std::endl;

    switch(selection) {
        case 0:
            bubbleSort<double>(arr, n);
            break;
        case 1:
            insertionSort<double>(arr, n);
            break;
        case 2:
            mergeSort<double>(arr, 0, n - 1);
            break;
        case 3:
            quickSort<double>(arr, 0, n);
            break;
        case 4:
            selectionSort<double>(arr, n);
            break;
    };
    
    std::cout << "After Sorting: " << std::endl;
    printArray<double>(arr, n);

    std::this_thread::sleep_for(std::chrono::milliseconds(2000));

    delete[] arr;
    return EXIT_SUCCESS;
}