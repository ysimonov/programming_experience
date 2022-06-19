#ifndef PRICECOMBINATIONFINDER_HPP
#define PRICECOMBINATIONFINDER_HPP

#include <vector>

// std::vector<std::vector<int>> getPriceCombinations(std::vector<std::vector<int>> item_prices, int total_price);
std::vector<std::vector<int>> priceCombinationsGenerator(std::vector<std::vector<int>> unique_item_prices, int total_price);

#endif