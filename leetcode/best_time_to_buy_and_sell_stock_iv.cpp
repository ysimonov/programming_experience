#include <iostream>
#include <numeric>
#include <vector>

using namespace std;

void print_vector(vector<int>& a) {
    if (a.empty()) return;
    if (a.size() == 1) {
        cout << a[0] << endl;
    }
    for (int i = 0; i < a.size() - 1; ++i) {
        cout << a[i] << ", ";
    }
    cout << a[a.size() - 1] << endl;
}

class Solution {
   public:
    void generateCombinations(
        vector<int>& prices,      // price list
        vector<int> combination,  // temporary array for current combination
        vector<int>& best_combination,  // temporary array for best combination
        int arr_size, int k, int index, int& best_sum, int i) {
        // current combination is ready,
        if (index == k || i >= arr_size) {
            // print_vector(combination);

            int current_sum =
                accumulate(combination.begin(), combination.end(), 0);
            if (current_sum > best_sum) {
                best_sum = current_sum;
                // cout << "best sum: " << best_sum << endl;
                best_combination = move(combination);
            }
            return;
        }

        // current is included, put next at next location
        combination[index] = prices[i];
        generateCombinations(prices, combination, best_combination, arr_size, k,
                             index + 1, best_sum, i + 1);

        // current is excluded, replace with next
        generateCombinations(prices, combination, best_combination, arr_size, k,
                             index, best_sum, i + 1);
    }

    int maxProfit(int k, vector<int>& prices) {
        // prices[i] is the price of the stock on i-th day
        // k is the maximum number of transactions that can be made
        // you must sell the stock before buying another one

        // loop over stock prices
        int num_prices = prices.size();
        int last_price_idx = num_prices - 1;

        // find start and stop idx
        // and move in increments

        int net_profit = 0;
        int idx_start = 0;
        vector<int> potential_transactions;
        for (int i = 0; i < last_price_idx; ++i) {
            if (i < idx_start) {
                continue;
            }
            const int& curr_price1 = prices[i];
            const int& next_price1 = prices[i + 1];
            if (curr_price1 > next_price1) {
                // shift the start idx
                idx_start = i + 1;
                if (idx_start == last_price_idx) {
                    // no profit
                    break;
                }
            } else {
                // find maximum price until it starts dropping again
                int idx_stop = -1;
                int price_start = prices[idx_start];
                for (int j = idx_start + 1; j < num_prices; ++j) {
                    const int& price_stop = prices[j];
                    if (price_stop < prices[j - 1]) {
                        break;
                    }
                    idx_stop = j;
                }
                // cout << "idx start: " << idx_start << endl;
                // cout << "idx stop: " << idx_stop << endl;
                if (idx_stop != -1) {
                    // net_profit += prices[idx_stop] - prices[idx_start];
                    int transaction = prices[idx_stop] - prices[idx_start];
                    potential_transactions.emplace_back(transaction);
                    idx_start = idx_stop + 1;
                    if (idx_start >= last_price_idx) {
                        break;
                    }
                }
            }
        }

        // check if any profit can be generated at all
        int num_transactions = potential_transactions.size();
        if (num_transactions == 0) {
            return 0;
        }

        // find maximum k transactions that would maximize profit
        // essentially that means that we need to find permutations
        // of successful transactions within the list of transactions
        vector<int> combination(k);
        vector<int> best_combination(k);

        generateCombinations(
            potential_transactions,  // price list
            combination,             // temporary array for current combination
            best_combination,        // temporary array for best combination
            num_transactions, k, 0, net_profit, 0);

        return net_profit;
    }
};

int main() {
    int k = 2;
    vector<int> prices = {3, 3, 5, 0, 0, 3, 1, 4};

    Solution sol;

    int res = sol.maxProfit(k, prices);

    cout << "Max profit: " << res << endl;

    return 0;
}