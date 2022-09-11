#include <string>
#include <vector>

using namespace std;

/* Problem statement

Given two strings word1 and word2, return the minimum number of operations
required to convert word1 to word2.

You have the following three operations permitted on a word:

Insert a character
Delete a character
Replace a character


Example 1:

Input: word1 = "horse", word2 = "ros"
Output: 3
Explanation:
horse -> rorse (replace 'h' with 'r')
rorse -> rose (remove 'r')
rose -> ros (remove 'e')


Example 2:

Input: word1 = "intention", word2 = "execution"
Output: 5
Explanation:
intention -> inention (remove 't')
inention -> enention (replace 'i' with 'e')
enention -> exention (replace 'n' with 'x')
exention -> exection (replace 'n' with 'c')
exection -> execution (insert 'u')


Constraints:

0 <= word1.length, word2.length <= 500
word1 and word2 consist of lowercase English letters.

*/

class Solution {
   public:
    int minDistance(string word1, string word2) {
        // Simple check
        int len1 = word1.length();
        int len2 = word2.length();

        if (len1 == 0) {
            return len2;
        } else if (len2 == 0) {
            return len1;
        }

        // Wagner–Fischer algorithm
        int M = len1 + 1;
        int N = len2 + 1;
        int sub_cost;

        auto dp = vector<int>(M * N);

        for (int i = 1; i < M; ++i) {
            dp[i * N] = i;
        }

        for (int j = 1; j < N; ++j) {
            dp[j] = j;
        }

        int im, jm;

        for (int j = 1; j < N; ++j) {
            jm = j - 1;
            for (int i = 1; i < M; ++i) {
                im = i - 1;

                // choose substitution cost
                if (word1[im] == word2[jm]) {
                    sub_cost = 0;
                } else {
                    sub_cost = 1;
                }

                // populate dynamic array (i, j) index
                dp[i * N + j] = min(min(dp[im * N + j] + 1, dp[i * N + jm] + 1),
                                    dp[im * N + jm] + sub_cost);
            }
        }
        return dp.back();
    }
};