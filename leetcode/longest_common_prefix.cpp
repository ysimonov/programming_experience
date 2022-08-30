#include <iostream>
#include <string>
#include <vector>

using namespace std;

/*
Example 1:

Input: strs = ["flower","flow","flight"]
Output: "fl"

Example 2:

Input: strs = ["dog","racecar","car"]
Output: ""
Explanation: There is no common prefix among the input strings.

Constraints:

1 <= strs.length <= 200
0 <= strs[i].length <= 200
strs[i] consists of only lowercase English letters.
*/

class Solution
{
  public:
    string longestCommonPrefix(vector<string> &strs)
    {
        auto number_of_strings = strs.size();
        if (number_of_strings == 1)
        {
            return strs[0];
        }
        else if (strs[0].length() == 0)
        {
            return "";
        }
        // find the length of the shortest string in the vector
        vector<int> lengths;
        int min_length = INT_MAX;
        const string &str0 = strs[0];
        char ch = str0.at(0);
        // check min length and
        for (auto &str : strs)
        {
            auto strl = str.length();
            if (strl == 0)
                return "";
            if (strl < min_length)
            {
                min_length = strl;
            }
            if (str.at(0) != ch)
            {
                return "";
            }
        }
        if (min_length == 1)
        {
            return {strs[0].at(0)};
        }
        // since the above part passed, it means that
        // all first characters for all strings are the same
        // so only need to check characters from position 2, until min_length;
        for (int pos = 1; pos < min_length; pos++)
        {
            ch = str0.at(pos);
            for (int i = 1; i < number_of_strings; i++)
            {
                const string &str = strs[i];
                if (str.at(pos) != ch)
                {
                    return str0.substr(0, pos);
                }
            }
        }
        return str0.substr(0, min_length);
    }
};

int main()
{
    vector<string> test_vector1 = {"dog", "racecar", "car"};
    vector<string> test_vector2 = {"flower", "flow", "flight"};
    auto sol = Solution();
    auto ch1 = sol.longestCommonPrefix(test_vector1);
    auto ch2 = sol.longestCommonPrefix(test_vector2);
    cout << "Solution 1: " << ch1 << endl;
    cout << "Solution 2: " << ch2 << endl;
    return EXIT_SUCCESS;
}