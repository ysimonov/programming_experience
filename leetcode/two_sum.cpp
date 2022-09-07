#include <iostream>
#include <unordered_map>
#include <vector>

using namespace std;

/*
Given an array of integers nums and an integer target, return indices of the two
numbers such that they add up to target.

You may assume that each input would have exactly one solution, and you may not
use the same element twice.

You can return the answer in any order.


Example 1:

Input: nums = [2,7,11,15], target = 9
Output: [0,1]
Explanation: Because nums[0] + nums[1] == 9, we return [0, 1].


Example 2:

Input: nums = [3,2,4], target = 6
Output: [1,2]


Example 3:

Input: nums = [3,3], target = 6
Output: [0,1]
*/

class Solution {
   public:
    vector<int> twoSum(vector<int> &nums, int target) {
        // copy values to unordered map
        unordered_map<int, vector<int>> nums_map;
        int idx = 0;
        for (const int &num : nums) {
            if (!nums_map.count(num)) {
                nums_map[num] = {};
            }
            nums_map[num].emplace_back(idx);
            idx += 1;
        }
        // iterate over values in vector and select
        // indices that would add up to target
        for (int i = 0; i < nums.size(); i++) {
            const int &num = nums[i];
            int target_num = target - num;
            // check if element is in the map
            if (nums_map.count(target_num)) {
                // retrieve index
                vector<int> &idx_vec = nums_map.find(target_num)->second;
                for (const auto &idx : idx_vec) {
                    if (idx != i) {
                        return {i, idx};
                    }
                }
            }
        }
        return {};
    }
};

int main() {
    auto sol = Solution();
    vector<int> test_vec = {3, 0, 1, 2, 3};
    int target = 6;
    auto res = sol.twoSum(test_vec, target);
    for (auto &val : res) {
        cout << val << endl;
    }
    return EXIT_SUCCESS;
}