#include <iostream>
#include <numeric>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

/*
Expected results:

Input: s = "III"
Output: 3
Explanation: III = 3.

Input: s = "LVIII"
Output: 58
Explanation: L = 50, V= 5, III = 3.

Input: s = "MCMXCIV"
Output: 1994
Explanation: M = 1000, CM = 900, XC = 90 and IV = 4.

Constraints:

1 <= s.length <= 15
s contains only the characters ('I', 'V', 'X', 'L', 'C', 'D', 'M').
It is guaranteed that s is a valid roman numeral in the range [1, 3999].
*/

class Solution {
   public:
    int romanToInt(string str) {
        // firstly check string length
        auto str_len = str.length();
        if (str_len == 0)
            return 0;
        unordered_map<char, int> dict = {{'I', 1}, {'V', 5}, {'X', 10}, {'L', 50}, {'C', 100}, {'D', 500}, {'M', 1000}};
        unordered_map<string, int> dict_combinations = {{"IV", 4}, {"IX", 9}, {"XL", 40}, {"XC", 90}, {"CD", 400}, {"CM", 900}};
        if (str_len == 1) {
            return dict[str.at(0)];
        } else {
            vector<int> total_values;
            int current_idx = 0;
            total_values.emplace_back(dict[str.at(0)]);
            for (int i = 1; i < str_len; i++) {
                // check if prev + curr make a valid combination
                std::string char_comb{str.at(i - 1), str.at(i)};
                // look for char_comb in dict_combinations
                auto it = dict_combinations.find(char_comb);
                // if combination was found, update previous value of the container
                if (it != dict_combinations.end()) {
                    total_values[current_idx] = dict_combinations[char_comb];
                } else {
                    // increment index
                    current_idx += 1;
                    // append new value for total container
                    total_values.emplace_back(dict[str.at(i)]);
                }
            }
            // accumulate all values (sum)
            int result = std::accumulate(total_values.begin(), total_values.end(), 0);
            return result;
        }
    }
};

int main() {
    auto sol = Solution();
    auto r1 = sol.romanToInt("III");
    auto r2 = sol.romanToInt("LVIII");
    auto r3 = sol.romanToInt("MCMXCIV");
    cout << r1 << " " << r2 << " " << r3 << endl;
    return EXIT_SUCCESS;
}