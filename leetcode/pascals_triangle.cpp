#include <vector>

using namespace std;

/* Problem statement

Given an integer numRows, return the first numRows of Pascal's triangle.

Example 1:

Input: numRows = 5
Output: [[1],[1,1],[1,2,1],[1,3,3,1],[1,4,6,4,1]]

Example 2:

Input: numRows = 1
Output: [[1]]

*/

class Solution {
   public:
    vector<vector<int>> generate(int numRows) {
        if (numRows == 1) {
            return {{1}};
        }
        vector<vector<int>> result({{1}});
        auto last_row = result.back();
        for (int i = 1; i < numRows; ++i) {
            vector<int> new_row{1};
            for (int j = 1; j < last_row.size(); ++j) {
                new_row.emplace_back(last_row[j - 1] + last_row[j]);
            }
            new_row.emplace_back(1);
            result.emplace_back(new_row);
            last_row = move(new_row);
        }
        return result;
    }
};