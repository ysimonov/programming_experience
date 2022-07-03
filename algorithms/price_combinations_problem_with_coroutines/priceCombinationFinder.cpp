#include <iostream>
#include <vector>
#include <list>
#include <chrono>
#include <numeric>      // std::accumulate

#include <boost/coroutine2/all.hpp>

using namespace std;
using namespace boost::coroutines2;

// compile: g++ priceCombinationFinder.cpp -o main -lboost_program_options-mt

template<typename T>
vector<T> slice(vector<T> &v, int m, int n)
{
    vector<T> vec;
    copy(v.begin() + m, v.begin() + n + 1, back_inserter(vec));
    return vec;
}

typedef coroutine<list<int>> coro_t;

void generateCombination(coro_t::push_type& yield, vector<vector<int>> listOfItems) {
    if (listOfItems.empty()) {
        yield({});
    } else {
        for(auto& itemPrice: listOfItems[0]) {
            for(auto& product: 
                    coro_t::pull_type(
                        fixedsize_stack(), 
                        bind(generateCombination, slice(listOfItems, 1, listOfItems.size()))
                    )
            ) {
                product.push_front(itemPrice);
                yield(product);
            }
        }
    }
}

list<int> getPriceCombination(vector<vector<int>> listOfItems, int totalPrice) {
    list<int> validPriceCombination;
    for (auto& priceCombination:
            coro_t::pull_type(
                fixedsize_stack(),
                bind(generateCombination, listOfItems)
            )
        ) {
            int totalCheck = 0;
            totalCheck = accumulate(priceCombination.begin(), priceCombination.end(), 0);
            if (totalCheck == totalPrice) {
                cout << "Found price combination" << endl;
                validPriceCombination = priceCombination;
                break;
            }
        }
    return validPriceCombination;
}

int main() {

    int totalPrice = 100;
    vector<vector<int>> listOfItems = {
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

    // get all unique price combinations
    auto begin = std::chrono::high_resolution_clock::now();

    // auto priceCombination = getPriceCombination(listOfItems, totalPrice);

    auto end = std::chrono::high_resolution_clock::now();
    std::cout << double(std::chrono::duration_cast<std::chrono::nanoseconds>(end-begin).count())/(1e9) << "s" << std::endl;
   
    // for(auto& price: priceCombination) {
    //     std::cout << price << " ";
    // }

    return EXIT_SUCCESS;
}