#include <iostream>
#include <vector>
#include <chrono>
#include <map>
#include <string>
#include <algorithm>
#include <unordered_set>
#include <set>
#include <numeric>

std::vector<std::vector<int>> getUniquePrices(std::vector<std::vector<int>> item_prices) {
    std::vector<std::vector<int>> unique_item_prices;
    for (auto it = item_prices.begin(), end = item_prices.end(); it != end; ++it) {
        auto values = *it;
        auto unique_price_set = std::set(values.begin(), values.end()); 
        std::vector<int> unique_prices(unique_price_set.begin(), unique_price_set.end());
        unique_item_prices.emplace_back(unique_prices);
    }
    // print prices
    // for (auto item: unique_item_prices) {
    //     std::cout << "[";
    //     for (auto price: item) {
    //         std::cout << price << ", ";
    //     } 
    //     std::cout << "]" << std::endl;
    // }
    return unique_item_prices;
}

std::vector<std::vector<int>> priceCombinationsGenerator(
    std::vector<std::vector<int>> unique_item_prices,
    int total_price
) {
    std::vector<std::vector<int>> results = {{}};
    std::vector<std::vector<int>> temp;
    for(int i=0; i<unique_item_prices.size(); i++) {
        temp.clear();
        for (auto res: results) {
            for (auto element: unique_item_prices[i]) {
                std::vector<int> new_vect;
                for (auto res_el: res) {
                    new_vect.emplace_back(res_el);
                }
                new_vect.emplace_back(element);
                temp.emplace_back(new_vect);
            }
        }
        results = std::move(temp);
    }

    std::vector<std::vector<int>> filtered_results;
    for(auto price_combination: results) {
        auto total_combination = std::accumulate(price_combination.begin(), price_combination.end(), 0);
        if (total_price == total_combination) {
            filtered_results.emplace_back(std::move(price_combination));
        }
    }

    return filtered_results;
}

std::vector<std::vector<int>> getPriceCombinations(std::vector<std::vector<int>> item_prices, int total_price) {

    // firstly get unique values from each item
    auto unique_item_prices = getUniquePrices(item_prices);
    auto price_combinations = priceCombinationsGenerator(unique_item_prices, total_price);

    return price_combinations;
}


int main() {

    std::map<std::string, std::vector<int>> item_dictionary;
    std::vector<std::vector<int>> item_list;
    int total_price = 100;

    item_dictionary["Cheeseburger"] = {3, 7, 5, 8, 1};
    item_dictionary["Coca-Cola"] = {4, 4};
    item_dictionary["Mac'n Cheese"] = {2, 4, 5};
    item_dictionary["Hawaiian Pizza"] = {10, 9};
    item_dictionary["Salty Popcorn"] = {1, 20};
    item_dictionary["Bubble Tea"] = {3, 7, 6};
    item_dictionary["Chocolate Protein Balls"] = {4, 6, 13, 25};
    item_dictionary["Weet-Bix"] = {1, 5, 10};
    item_dictionary["Curry Chicken"] = {6, 8};
    item_dictionary["Tuna Sandwich"] = {22, 35, 20};
    item_dictionary["Banana Bread"] = {12, 2, 10};

    // get all the prices from item_dictionary and convery into vector
    std::vector<std::string> item_names;
    for (auto item: item_dictionary) {
        std::cout << "Prices for " << item.first << ": ";
        for (auto price: item.second) {
            std::cout << price << " ";
        }
        std::cout << std::endl;
        item_list.emplace_back(item.second);
        item_names.emplace_back(item.first);
    }

    // get all unique price combinations
    auto begin = std::chrono::high_resolution_clock::now();
    auto price_combinations = getPriceCombinations(item_list, total_price);
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << double(std::chrono::duration_cast<std::chrono::nanoseconds>(end-begin).count())/(1e9) << "s" << std::endl;
    std::cout << "Number of price combinations: " << price_combinations.size() << std::endl;

    // print first 5 price combinations
    int i = 0;
    std::cout << "Printing first 5 valid combinations: " << "\n\n";
    for(auto price_combination: price_combinations) {
        for (int i=0; i<price_combination.size(); i++) {
            std::cout << item_names[i] << ": " << price_combination[i] << std::endl;
        }
        std::cout << std::endl;
        i += 1;
        if (i == 5)
            break;
    }

    return EXIT_SUCCESS;
}
