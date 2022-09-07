#include <iostream>
#include <unordered_map>
#include <vector>

using namespace std;

/*
You are given two string arrays words1 and words2.

A string b is a subset of string a if every letter in b occurs in a including multiplicity.

For example, "wrr" is a subset of "warrior" but is not a subset of "world".
A string a from words1 is universal if for every string b in words2, b is a subset of a.

Return an array of all the universal strings in words1. You may return the answer in any order.


Example 1:

Input: words1 = ["amazon","apple","facebook","google","leetcode"], words2 = ["e","o"]
Output: ["facebook","google","leetcode"]


Example 2:

Input: words1 = ["amazon","apple","facebook","google","leetcode"], words2 = ["l","e"]
Output: ["apple","google","leetcode"]

*/

class Solution {
   public:
    vector<string> wordSubsets(vector<string> &words1, vector<string> &words2) {
        vector<string> result;

        // create dictionary of characters in words2 (including multiplicity)
        unordered_map<char, int> chars2_dict = {};
        for (const auto &word2 : words2) {
            unordered_map<char, int> multiplicity_dict;
            for (const auto &ch2 : word2) {
                if (!multiplicity_dict.count(ch2)) {
                    multiplicity_dict[ch2] = 1;
                } else {
                    multiplicity_dict[ch2] += 1;
                }
            }
            for (const auto &it : multiplicity_dict) {
                const auto &ch2 = it.first;
                const auto &multiplicity = it.second;
                if (chars2_dict.count(ch2)) {
                    chars2_dict[ch2] = max(chars2_dict.find(ch2)->second, multiplicity);
                } else {
                    chars2_dict[ch2] = multiplicity;
                }
            }
        }

        // for (const auto& it : chars2_dict) {
        //     cout << it.first << " " << it.second << endl;
        // }

        vector<unordered_map<char, int>> words1_dict;
        for (const auto &word1 : words1) {
            unordered_map<char, int> chars1_dict = {};
            for (const auto &ch1 : word1) {
                if (!chars1_dict.count(ch1)) {
                    chars1_dict[ch1] = 1;
                } else {
                    chars1_dict[ch1] += 1;
                }
            }
            words1_dict.emplace_back(chars1_dict);
        }

        // for (const auto& it1 : words1_dict) {
        //     for (const auto& it2: it1) {
        //         cout << it2.first << " " << it2.second << endl;
        //     }
        //     cout << endl;
        // }

        for (int i = 0; i < words1.size(); i++) {
            const auto &chars1_dict = words1_dict[i];
            bool found_all_chars = true;
            for (const auto &it2 : chars2_dict) {
                const char &ch2 = it2.first;
                if (chars1_dict.count(ch2)) {
                    auto multiplicity = it2.second;
                    if (chars1_dict.find(ch2)->second < multiplicity) {
                        found_all_chars = false;
                        break;
                    }
                } else {
                    found_all_chars = false;
                    break;
                }
            }
            if (found_all_chars) {
                result.emplace_back(words1[i]);
            }
        }

        return result;
    }
};

int main() {
    auto sol = Solution();
    vector<string> words1 = {"amazon", "apple", "facebook", "google", "leetcode"};
    vector<string> words2 = {"e", "oo"};
    auto res = sol.wordSubsets(words1, words2);
    for (const auto &word : res) {
        cout << word << " ";
    }
    cout << endl;
    return EXIT_SUCCESS;
}