/*
Given an integer n, return a string array answer (1-indexed) where:

answer[i] == "FizzBuzz" if i is divisible by 3 and 5.
answer[i] == "Fizz" if i is divisible by 3.
answer[i] == "Buzz" if i is divisible by 5.
answer[i] == i (as a string) if none of the above conditions are true.


Example 1:

Input: n = 3
Output: ["1","2","Fizz"]
Example 2:

Input: n = 5
Output: ["1","2","Fizz","4","Buzz"]
Example 3:

Input: n = 15
Output: ["1","2","Fizz","4","Buzz","Fizz","7","8","Fizz","Buzz","11","Fizz","13","14","FizzBuzz"]


Constraints:

1 <= n <= 10^4
*/

#include <iostream>
#include <string>
#include <vector>

using namespace std;

using std::cout;
using std::endl;

class Solution {
   public:
    Solution() = default;
    vector<string> fizzBuzz(int n) {
        vector<string> result;
        result.reserve(n);
        int k = min(n + 1, 10001);
        for (int i = 1; i < k; i++) {
            if ((i % 3 == 0) && (i % 5 == 0)) {
                result.emplace_back("FizzBuzz");
            } else if (i % 3 == 0) {
                result.emplace_back("Fizz");
            } else if (i % 5 == 0) {
                result.emplace_back("Buzz");
            } else {
                auto temp = to_string(i);
                cout << temp << endl;
                result.emplace_back(temp);
            }
        }
        return result;
    }
};

int main() {
    Solution sol;
    auto result = sol.fizzBuzz(10);

    for (auto &it : result) {
        cout << it << '\n';
    }

    return EXIT_SUCCESS;
}