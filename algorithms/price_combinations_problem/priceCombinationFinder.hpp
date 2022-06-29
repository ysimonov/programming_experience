#ifndef PRICECOMBINATIONFINDER_HPP
#define PRICECOMBINATIONFINDER_HPP

#include <vector>

std::vector<std::vector<int>> getPriceCombinations(const std::vector<std::vector<int>>& listOfPrices);
std::vector<int> getValidPriceCombination(const std::vector<std::vector<int>>& listOfPrices, const int& totalPrice);

#endif