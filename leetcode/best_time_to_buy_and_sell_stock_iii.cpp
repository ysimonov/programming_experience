#include <limits>
#include <vector>
using namespace std;

class Solution {
   public:
    int maxProfit(vector<int>& prices) {
        int n = prices.size();

        // cannot make profit from only one transaction
        if (n <= 1) {
            return 0;
        }

        // dynamic arrays
        vector<int> even_transaction_profits(n);
        vector<int> odd_transaction_profits(n);
        vector<int>* current_transaction_profits = nullptr;
        vector<int>* previous_transaction_profits = nullptr;

        // number of transcations
        int k = 1;

        // populate dynamic array
        for (int t = 0; t < k + 1; ++t) {
            int max_profit = numeric_limits<int>::min();

            if (t % 2 == 1) {
                current_transaction_profits = &odd_transaction_profits;
                previous_transaction_profits = &even_transaction_profits;
            } else {
                current_transaction_profits = &even_transaction_profits;
                previous_transaction_profits = &odd_transaction_profits;
            }

            for (int d = 1; d < n; ++d) {
                max_profit =
                    max(max_profit,
                        (*previous_transaction_profits)[d - 1] - prices[d - 1]);
                (*current_transaction_profits)[d] =
                    max((*current_transaction_profits)[d - 1],
                        max_profit + prices[d]);
            }
        }

        current_transaction_profits = nullptr;
        previous_transaction_profits = nullptr;

        if (k % 2 == 0) {
            return even_transaction_profits.back();
        } else {
            return odd_transaction_profits.back();
        }
    }
};