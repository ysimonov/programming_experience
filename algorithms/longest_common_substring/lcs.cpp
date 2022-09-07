#include <algorithm>
#include <cstring>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

/*
    This program finds the length of the longest common substring between two strings
    two strings, for example:
    STRING 1: Good Morning How Are You?
    STRING 2: good morning how are you!
    RESULT: ood orning ow re ou
    LENGTH: 19 (including white spaces)
*/

class LongestCommonSubstring {
    std::string string1;
    std::string string2;
    size_t length1;
    size_t length2;

   public:
    LongestCommonSubstring(std::string str1, std::string str2, size_t len1, size_t len2);
    ~LongestCommonSubstring();

    int length = 0;         // length of the longest common Substring
    std::string substring;  // longest common character sequence

    int GetSubstringLength();
    std::string GetSubstring();

   private:
    std::unique_ptr<std::unique_ptr<int[]>[]> dist;
    int right_row_anchor;
    int right_col_anchor;
};

LongestCommonSubstring::LongestCommonSubstring(std::string str1, std::string str2, size_t len1, size_t len2) {
    string1 = str1;
    string2 = str2;
    length1 = len1;
    length2 = len2;
}

LongestCommonSubstring::~LongestCommonSubstring() {
    if (dist != nullptr) {
        for (int i = 0; i < length1; i++)
            dist[i].reset();
        dist.reset();
    }
}

int LongestCommonSubstring::GetSubstringLength() {
    if (length1 == 0)
        return length2;
    else if (length2 == 0)
        return length1;

    int i, j, im, jm;
    size_t M = length1 + 1;
    size_t N = length2 + 1;

    // space allocation and initialization
    dist = std::make_unique<std::unique_ptr<int[]>[]>(M);
    for (i = 0; i < M; i++) {
        dist[i] = std::make_unique<int[]>(N);
        for (j = 0; j < N; j++) {
            dist[i][j] = 0;
        }
    }

    // longest common Substring algorithm (finding length)
    for (j = 1; j < N; j++) {
        jm = j - 1;
        for (i = 1; i < M; i++) {
            im = i - 1;
            if (string1[im] == string2[jm]) {
                dist[i][j] = dist[im][jm] + 1;
                if (length < dist[i][j]) {
                    length = dist[i][j];
                    right_row_anchor = i;
                    right_col_anchor = j;
                }
            }
        }
    }
    return length;
}

std::string LongestCommonSubstring::GetSubstring() {
    std::string default_message = "Firstly calculate length of Substring!";
    if (dist == nullptr)
        return default_message;

    // finding sequence of characters
    int row = right_row_anchor;
    int col = right_col_anchor;
    while (dist[row][col] != 0) {
        substring += string1[row - 1];
        row--;
        col--;
    }
    std::reverse(substring.begin(), substring.end());
    return substring;
}

int main() {
    std::string str1, str2;
    size_t str1_len, str2_len;

    int longest_common_substring_len;
    std::string substring_string;

    std::cout << "Enter first string: ";
    std::getline(std::cin, str1);
    str1_len = str1.length();

    std::cout << "Enter second string: ";
    std::getline(std::cin, str2);
    str2_len = str2.length();

    LongestCommonSubstring substringObj(str1, str2, str1_len, str2_len);
    longest_common_substring_len = substringObj.GetSubstringLength();
    substring_string = substringObj.GetSubstring();

    std::cout << "Length of the longest common Substring: " << longest_common_substring_len << std::endl;
    std::cout << "Substring: " << substring_string << std::endl;

    return EXIT_SUCCESS;
}