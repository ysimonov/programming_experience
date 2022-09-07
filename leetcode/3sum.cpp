#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

using namespace std;

/*
Given an integer array nums, return all the triplets [nums[i], nums[j], nums[k]]
such that i != j, i != k, and j != k, and nums[i] + nums[j] + nums[k] == 0.

Notice that the solution set must not contain duplicate triplets.


Example 1:

Input: nums = [-1,0,1,2,-1,-4]
Output: [[-1,-1,2],[-1,0,1]]
Explanation:
nums[0] + nums[1] + nums[2] = (-1) + 0 + 1 = 0.
nums[1] + nums[2] + nums[4] = 0 + 1 + (-1) = 0.
nums[0] + nums[3] + nums[4] = (-1) + 2 + (-1) = 0.
The distinct triplets are [-1,0,1] and [-1,-1,2].
Notice that the order of the output and the order of the triplets does not matter.


Example 2:

Input: nums = [0,1,1]
Output: []
Explanation: The only possible triplet does not sum up to 0.


Example 3:

Input: nums = [0,0,0]
Output: [[0,0,0]]
Explanation: The only possible triplet sums up to 0.


Constraints:

3 <= nums.length <= 3000
-105 <= nums[i] <= 105
*/

class Solution {
   public:
    struct hashFunction {
        size_t operator()(const pair<int, int> &x) const {
            return x.first ^ x.second;
        }
    };
    vector<vector<int>> threeSum(vector<int> &nums) {
        int number_of_numbers = nums.size();
        vector<vector<int>> result;

        unordered_map<int, unordered_set<int>> unique_numbers;
        for (int i = 0; i < number_of_numbers; i++) {
            const auto &num = nums[i];
            if (!unique_numbers.count(num)) {
                unique_numbers[num] = {};
            }
            unique_numbers[num].insert(i);
        }

        unordered_map<pair<int, int>, int, hashFunction> uniqueCombinations;
        for (int i = 0; i < number_of_numbers; i++) {
            const int &num1 = nums[i];
            for (int j = 0; j < number_of_numbers; j++) {
                if (i == j)
                    continue;
                const int &num2 = nums[j];
                auto sum = num1 + num2;
                auto num3 = -sum;
                if (unique_numbers.count(num3)) {
                    const auto &unique_idxs = unique_numbers[num3];
                    if (unique_idxs.size() == 2) {
                        auto cond1 = unique_idxs.count(i);
                        auto cond2 = unique_idxs.count(j);
                        if (cond1 && cond2) {
                            continue;
                        }
                    } else if ((unique_idxs.size() == 1) && ((num1 == num3) || (num2 == num3))) {
                        continue;
                    }

                    auto pair12 = make_pair(num1, num2);
                    if (!uniqueCombinations.count(pair12)) {
                        result.push_back({num1, num2, num3});

                        auto pair21 = make_pair(num2, num1);
                        auto pair13 = make_pair(num1, num3);
                        auto pair31 = make_pair(num3, num1);
                        auto pair23 = make_pair(num2, num3);
                        auto pair32 = make_pair(num3, num2);

                        uniqueCombinations[pair12] = num3;
                        uniqueCombinations[pair21] = num3;

                        uniqueCombinations[pair13] = num2;
                        uniqueCombinations[pair31] = num2;

                        uniqueCombinations[pair23] = num1;
                        uniqueCombinations[pair32] = num1;
                    }
                }
            }
        }
        return result;
    }
};
int main() {
    Solution sol;
    vector<int> a = {-1, 0, 1, 2, -1, -4};
    auto result = sol.threeSum(a);

    for (auto &vec : result) {
        for (auto &it : vec) {
            cout << it << ", ";
        }
        cout << endl;
    }

    return EXIT_SUCCESS;
}