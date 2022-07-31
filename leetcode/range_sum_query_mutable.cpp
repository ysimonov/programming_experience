#include <iostream>
#include <vector>
#include <numeric>

using namespace std;

/*

Example 1:

Input
["NumArray", "sumRange", "update", "sumRange"]
[[[1, 3, 5]], [0, 2], [1, 2], [0, 2]]
Output
[null, 9, null, 8]

Explanation
NumArray numArray = new NumArray([1, 3, 5]);
numArray.sumRange(0, 2); // return 1 + 3 + 5 = 9
numArray.update(1, 2);   // nums = [1, 2, 5]
numArray.sumRange(0, 2); // return 1 + 2 + 5 = 8

Constraints:

1 <= nums.length <= 3 * 104
-100 <= nums[i] <= 100
0 <= index < nums.length
-100 <= val <= 100
0 <= left <= right < nums.length
At most 3 * 104 calls will be made to update and sumRange.

*/

class NumArray {
public:
    vector<int> nums;
    int init_sum = 0;
    int size = 0;

    NumArray(vector<int>& nums_) {
        nums = nums_;
        init_sum = accumulate(nums.begin(), nums.end(), 0);
        size = nums_.size();
    }
    
    void update(int index, int val) {
        init_sum += val - nums[index];
        nums[index] = val;
    }
    
    int sumRange(int left, int right) {

        if (left == right)
            return nums[left];            

        int left_sum = 0;
        if (left > 0)
            left_sum = accumulate(nums.begin(), nums.begin() + left, 0);

        int right_sum = 0;
        if (right < size)
            right_sum = accumulate(nums.begin() + right + 1, nums.end(), 0);
    
        int partial_sum = init_sum - left_sum - right_sum;
        return partial_sum;
    }
};

int main() {
    
    vector<int> nums = {1, 3, 5};
    auto numArray = NumArray(nums);

    auto res = numArray.sumRange(0, 2); // return 1 + 3 + 5 = 9
    cout << "Sum Range (1): " << res << endl;

    numArray.update(1, 2);   // nums = [1, 2, 5]

    res = numArray.sumRange(0, 2); // return 1 + 2 + 5 = 8
    cout << "Sum Range (2): " << res << endl;

    return EXIT_SUCCESS;
}