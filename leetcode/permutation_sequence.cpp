#include <algorithm>
#include <iterator>
#include <ostream>
#include <sstream>
#include <string>
#include <vector>

/*
The set [1, 2, 3, ..., n] contains a total of n! unique permutations.

By listing and labeling all of the permutations in order, we get the following
sequence for n = 3:

"123"
"132"
"213"
"231"
"312"
"321"
Given n and k, return the kth permutation sequence.


Example 1:

Input: n = 3, k = 3
Output: "213"


Example 2:

Input: n = 4, k = 9
Output: "2314"


Example 3:

Input: n = 3, k = 1
Output: "123"


Constraints:

1 <= n <= 9
1 <= k <= n!

*/

class Solution {
   public:
    std::string getPermutation(int n, int k) {
        std::vector<int> sequence;
        sequence.reserve(n);
        int p = n + 1;
        int i = 1;
        for (i = 1; i < p; ++i) {
            sequence.emplace_back(i);
        }

        int m = k - 1;
        if (m > 0) {
            i = 0;
            do {
                std::next_permutation(sequence.begin(), sequence.end());
                ++i;
            } while (i != m);
        }

        std::stringstream result;
        std::copy(sequence.begin(), sequence.end(),
                  std::ostream_iterator<int>(result, ""));
        return std::move(result.str());
    }
};