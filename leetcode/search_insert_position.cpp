#include<iostream>
#include<vector>

using namespace std;

class Solution {
public:
    int searchInsert(vector<int>& nums, int target) {
        int size = nums.size();
        
        // empty vector, insert in the first position
        if (size == 0)
            return 0;
        
        // target is smaller than min element
        int idx_l = 0;
        if (target < nums[idx_l])
            return 0;
        
        // target is larger than max element
        int idx_r = size - 1;
        if (target > nums[idx_r])
            return idx_r + 1;

        
        int idx_m = idx_l + (idx_r - idx_l) / 2;
        while (idx_l <= idx_r) {
            const int& num = nums[idx_m];
            if (target == num) {
                return idx_m;
            } else if (target > num) {
                idx_l = idx_m + 1;
            } else {
                idx_r = idx_m - 1;
            }
            idx_m = idx_l + (idx_r - idx_l) / 2;
        }
        return idx_m;
    }
};


int main() {
    auto sol = Solution();
    vector<int> test_vec = {1, 1, 5, 60, 61, 61, 62, 120, 140, 155};
    int target = 62;
    auto res = sol.searchInsert(test_vec, target);
    cout << "Insertion position: " << res << endl;
    return EXIT_SUCCESS;
}