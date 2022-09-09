#include <vector>

using namespace std;

class Solution {
   public:
    template <typename T>
    void swap(T& t1, T& t2) {
        T temp = move(t1);
        t1 = move(t2);
        t2 = move(temp);
    }

    vector<vector<int>> permute(vector<int>& nums) {
        // generating permutations of digits using the Non-Recursive Heaps
        // Algorithm
        const auto& nums_len{nums.size()};

        vector<int> c(nums_len, 0);

        vector<vector<int>> permutations;
        permutations.emplace_back(nums);

        int i = 1;
        while (i < nums_len) {
            if (c[i] < i) {
                if (i % 2 == 0) {
                    swap<int>(nums[0], nums[i]);
                } else {
                    swap<int>(nums[c[i]], nums[i]);
                }
                permutations.emplace_back(nums);
                c[i]++;
                i = 1;
            } else {
                c[i] = 0;
                i++;
            }
        }

        return permutations;
    }
};