#include "priceCombinationFinder.hpp"

#include <iostream>

// Compile: g++ -std=c++11 -O2 main.cpp priceCombinationFinder.cpp -o main

void print(std::vector<int> v) {
    for(auto& e : v) {
        std::cout << e << " ";
    }
    std::cout << std::endl;
}

int main() {
    int totalPrice = 100;
    std::vector<std::vector<int>> listOfPrices = {
        {3, 7, 5, 8, 1},
        {4},
        {2, 4, 5},
        {10, 9},
        {1, 20},
        {3, 7, 6},
        {4, 6, 13, 25},
        {1, 5, 10},
        {6, 8},
        {22, 35, 20},
        {12, 2, 10}
    };
    auto validPriceCombination = getValidPriceCombination(listOfPrices, totalPrice);
    print(validPriceCombination);
    return EXIT_SUCCESS;
}