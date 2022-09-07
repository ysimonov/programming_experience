#include <iostream>
#include <string>
#include <unordered_map>

using namespace std;

/*
Given two strings ransomNote and magazine,
return true if ransomNote can be constructed by using
the letters from magazine and false otherwise.

Each letter in magazine can only be used once in ransomNote.


Example 1:

Input: ransomNote = "a", magazine = "b"
Output: false


Example 2:

Input: ransomNote = "aa", magazine = "ab"
Output: false


Example 3:

Input: ransomNote = "aa", magazine = "aab"
Output: true
*/

class Solution {
   public:
    bool canConstruct(string ransomNote, string magazine) {
        unordered_map<char, int> dict;
        for (char &ch : magazine) {
            auto it = dict.find(ch);
            if (it != dict.end()) {
                it->second += 1;
            } else {
                dict[ch] = 1;
            }
        }
        for (char &ch : ransomNote) {
            auto it = dict.find(ch);
            if (it != dict.end()) {
                if (it->second > 0) {
                    it->second -= 1;
                } else {
                    return false;
                }
            } else {
                return false;
            }
        }
        return true;
    }
};

int main() {
    auto sol = Solution();
    string ransomNote = "aa";
    string magazine = "aab";
    bool res = sol.canConstruct(ransomNote, magazine);
    if (res)
        cout << "Can construct ransomNote " << ransomNote << " from magazine " << magazine << endl;
    else
        cout << "Cannot construct ransomNote " << ransomNote << " from magazine " << magazine << endl;
    return EXIT_SUCCESS;
}