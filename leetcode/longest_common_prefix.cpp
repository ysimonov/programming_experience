#include <iostream>
#include <vector>
#include <string>

using namespace std;

class Solution {
public:
    string longestCommonPrefix(vector<string>& strs) {
        // find the length of the shortest string in the vector
        vector<int> lengths;
        int min_length = INT_MAX;
        char ch = strs[0].at(0);
        // check min length and 
        auto number_of_strings = strs.size();
        for (int i = 1; i < number_of_strings; i++) {
            const string& str = strs[i];
            auto strl = str.length();
            if (strl < min_length) {
                min_length = strl;
            }
            if (str.at(0) != ch) {
                return "";
            }
        }
        // since the above part passed, it means that
        // all first characters for all strings are the same
        // so only need to check characters from position 2, until min_length;
        for (int pos = 1; pos < min_length; pos++) {
            ch = strs[0].at(pos);
            for (int i = 1; i < number_of_strings; i++) {
                const string& str = strs[i];
                if (str.at(pos) != ch) {
                    return str.substr(0, pos);
                }
            }
        }
        return strs[0].substr(0, min_length); 
    }
};

int main() {
    vector<string> test_vector1 = {"dog", "racecar", "car"};
    vector<string> test_vector2 = {"flower","flow","flight"};
    auto sol = Solution();
    auto ch1 = sol.longestCommonPrefix(test_vector1);
    auto ch2 = sol.longestCommonPrefix(test_vector2);
    cout << "Solution 1: " << ch1 << endl;
    cout << "Solution 2: " << ch2 << endl;
    return EXIT_SUCCESS;
}