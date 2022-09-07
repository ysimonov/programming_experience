#include <iostream>
#include <vector>

using namespace std;

/*
Given an integer array nums sorted in non-decreasing order,
remove the duplicates in-place such that each unique element appears only once.
The relative order of the elements should be kept the same.

Since it is impossible to change the length of the array in some languages,
you must instead have the result be placed in the first part of the array nums.
More formally, if there are k elements after removing the duplicates,
then the first k elements of nums should hold the final result.
It does not matter what you leave beyond the first k elements.

Return k after placing the final result in the first k slots of nums.

Do not allocate extra space for another array.
You must do this by modifying the input array in-place with O(1) extra memory.
*/

class Solution {
   public:
    int removeDuplicates(vector<int> &nums) {
        size_t size = nums.size();
        if (size <= 1) {
            return size;
        }
        int i = 1;
        auto num = nums[0];
        while (i != nums.size()) {
            if (nums[i] == num) {
                vector<int>::iterator it = nums.begin() + i;
                nums.erase(it);
            } else {
                num = nums[i];
                i += 1;
            }
        }
        int k = nums.size();
        return k;
    }
};

int main() {
    auto sol = Solution();
    vector<int> nums = {0, 0, 1, 1, 1, 2, 2, 3, 3, 4};
    auto k = sol.removeDuplicates(nums);
    cout << "Size: " << k << endl;
    return EXIT_SUCCESS;
}