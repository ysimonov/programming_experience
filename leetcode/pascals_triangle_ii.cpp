#include <cmath>
#include <vector>
using namespace std;

class Solution {
   public:
    vector<int> getRow(int rowIndex) {
        // row relation is nCk = nC(k-1) * (n + 1 - k) / k
        // can be calculated up to N / 2 - th element
        // nC0 = 1 and nCn = 1

        if (rowIndex == 0) {
            return {1};
        }

        int nCk = 1;
        vector<int> result({nCk});
        int N = rowIndex + 1;  // number of elements within this row
        int M = N / 2;

        for (int i = 1; i < M; ++i) {
            int nCi = round(nCk * ((double)(rowIndex + 1 - i)) / i);
            result.emplace_back(nCi);
            nCk = nCi;
        }

        auto complete = result;

        if (rowIndex % 2 == 0) {
            int nCi = round(nCk * ((double)(rowIndex + 1 - M) / M));
            result.emplace_back(nCi);
        }

        complete.insert(complete.end(), result.rbegin(), result.rend());
        return complete;
    }
};