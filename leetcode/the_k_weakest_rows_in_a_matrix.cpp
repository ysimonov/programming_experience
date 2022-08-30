#include <iostream>
#include <queue>
#include <utility>

/*
You are given an m x n binary matrix mat of 1's (representing soldiers)
and 0's (representing civilians). The soldiers are positioned in front
of the civilians. That is, all the 1's will appear to the left of all the 0's in each row.

A row i is weaker than a row j if one of the following is true:

The number of soldiers in row i is less than the number of soldiers in row j.
Both rows have the same number of soldiers and i < j.
Return the indices of the k weakest rows in the matrix ordered from weakest to strongest.

Example 1:

Input: mat =
[[1,1,0,0,0],
 [1,1,1,1,0],
 [1,0,0,0,0],
 [1,1,0,0,0],
 [1,1,1,1,1]],
k = 3
Output: [2,0,3]

Explanation:
The number of soldiers in each row is:
- Row 0: 2
- Row 1: 4
- Row 2: 1
- Row 3: 2
- Row 4: 5
The rows ordered from weakest to strongest are [2,0,3,1,4].


Example 2:

Input: mat =
[[1,0,0,0],
 [1,1,1,1],
 [1,0,0,0],
 [1,0,0,0]],
k = 2
Output: [0,2]

Explanation:
The number of soldiers in each row is:
- Row 0: 1
- Row 1: 4
- Row 2: 1
- Row 3: 1
The rows ordered from weakest to strongest are [0,2,3,1].


Constraints:

m == mat.length
n == mat[i].length
2 <= n, m <= 100
1 <= k <= m
matrix[i][j] is either 0 or 1.
*/

#define pp pair<int, int>

using namespace std;

class Solution
{
  public:
    vector<int> kWeakestRows(vector<vector<int>> &mat, int k)
    {

        // queue with elements in increasing order
        // using compare function inside declaration
        priority_queue<pp, vector<pp>, greater<pp>> soldier_count;

        int i = 0;
        for (const auto &row : mat)
        {
            int number_of_soldiers = 0;
            for (const auto &el : row)
            {
                if (!el)
                    break;
                number_of_soldiers += el;
            }
            soldier_count.push(make_pair(number_of_soldiers, i));
            i++;
        }

        vector<int> result;
        i = 0;
        while (!soldier_count.empty() && i < k)
        {
            const pp count = soldier_count.top();
            soldier_count.pop();
            // cout << count.first << " " << count.second << endl;
            result.emplace_back(count.second);
            i++;
        }

        return result;
    }
};

int main()
{
    auto sol = Solution();
    vector<vector<int>> mat = {{1, 1, 0, 0, 0}, {1, 1, 1, 1, 0}, {1, 0, 0, 0, 0}, {1, 1, 0, 0, 0}, {1, 1, 1, 1, 1}};
    int k = 3;
    auto res = sol.kWeakestRows(mat, k);
    for (auto r : res)
    {
        cout << r << " ";
    }
    return EXIT_SUCCESS;
}