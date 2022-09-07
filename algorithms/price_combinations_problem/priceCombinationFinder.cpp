#include <algorithm>
#include <iostream>
#include <numeric>
#include <vector>

// compile: g++ -std=c++11 -O2 priceCombinationFinder.cpp -o priceCombinationFinder

std::vector<std::vector<int>> getPriceCombinations(const std::vector<std::vector<int>>& listOfPrices) {
    if (listOfPrices.empty()) {
        return {};
    }
    std::vector<std::vector<int>> priceCombinations;
    for (auto& price : listOfPrices[0]) {
        priceCombinations.push_back({price});
    }
    std::vector<std::vector<int>> temp;
    for (size_t i = 1; i < listOfPrices.size(); ++i) {
        temp.clear();
        for (auto& partialCombination : priceCombinations) {
            for (auto& sublistOfPrices : listOfPrices[i]) {
                auto elementTemp = partialCombination;
                elementTemp.emplace_back(sublistOfPrices);
                temp.emplace_back(elementTemp);
            }
        }
        priceCombinations = std::move(temp);
    }
    return priceCombinations;
}

std::vector<int> getValidPriceCombination(const std::vector<std::vector<int>>& listOfPrices, const int& totalPrice) {
    std::vector<int> successfulPriceCombination;
    auto allPriceCombinations = getPriceCombinations(listOfPrices);
    for (auto& priceCombination : allPriceCombinations) {
        auto totalTemp = std::accumulate(priceCombination.begin(), priceCombination.end(), 0);
        if (totalTemp == totalPrice) {
            successfulPriceCombination = priceCombination;
            break;
        }
    }
    return successfulPriceCombination;
}