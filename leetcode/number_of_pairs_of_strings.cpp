#include<iostream>
#include<string>
#include<vector>

using namespace std;

/*
Given an array of digit strings nums and a digit string target, 
return the number of pairs of indices (i, j) (where i != j) 
such that the concatenation of nums[i] + nums[j] equals target.

 
Example 1:

Input: nums = ["777","7","77","77"], target = "7777"
Output: 4
Explanation: Valid pairs are:
- (0, 1): "777" + "7"
- (1, 0): "7" + "777"
- (2, 3): "77" + "77"
- (3, 2): "77" + "77"


Example 2:

Input: nums = ["123","4","12","34"], target = "1234"
Output: 2
Explanation: Valid pairs are:
- (0, 1): "123" + "4"
- (2, 3): "12" + "34"


Example 3:

Input: nums = ["1","1","1"], target = "11"
Output: 6
Explanation: Valid pairs are:
- (0, 1): "1" + "1"
- (1, 0): "1" + "1"
- (0, 2): "1" + "1"
- (2, 0): "1" + "1"
- (1, 2): "1" + "1"
- (2, 1): "1" + "1"
 

Constraints:

2 <= nums.length <= 100
1 <= nums[i].length <= 100
2 <= target.length <= 100
nums[i] and target consist of digits.
nums[i] and target do not have leading zeros.
*/

class Solution {
public:
    int numOfPairs(vector<string>& nums, string target) {
        // get length of the target string
        int target_len = target.length();
        // get lengths of each number string in nums
        int number_of_nums = nums.size();
        vector<int> num_lengths;
        for (const auto& num : nums) {
            num_lengths.emplace_back(num.length());
        }
        // calculate number of valid combinations
        int number_of_combinations = 0;
        for (int i = 0; i < number_of_nums; i++) {
            // check if this combination is possible
            const auto& len1 = num_lengths[i];
            if (len1 > target_len)
                continue;
            const auto& s1 = nums[i];
            for (int j = 0; j < number_of_nums; j++) {
                // indices should not repeat
                if (i == j)
                    continue;
                const auto& len2 = num_lengths[j];
                // length of target = length of string 1 + length of string 2
                if (len1 + len2 != target_len)
                    continue;
                const auto& s2 = nums[j];
                string s12 = s1 + s2;
                bool failed = false;
                // iterate over every character in the concatenated string
                for (int k = 0; k < target_len; k++) {
                    if (s12[k] != target[k]) {
                        failed = true;
                        break;
                    }
                }
                if (!failed)
                    number_of_combinations++;
            }
        }
        return number_of_combinations;
    }
};

int main() {
    auto sol = Solution();
    vector<string> nums({"777", "7", "77", "77"});
    string target("7777");
    auto res = sol.numOfPairs(nums, target);
    cout << "Number of combinations: " << res << endl;
    return EXIT_SUCCESS;
}