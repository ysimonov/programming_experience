#include <iostream>
#include <string>
#include <algorithm>
#include <memory>
#include <vector>
#include <cstring>

/*
    This program finds the length of the longest sequence of characters between
    two strings, for example:
    STRING 1: Good Morning How Are You?
    STRING 2: good morning how are you!
    RESULT: ood orning ow re ou
    LENGTH: 19 (including white spaces)
*/


class LongestCommonSubsequence {

    std::string string1;
    std::string string2;
    size_t length1;
    size_t length2;

    public:

        LongestCommonSubsequence(std::string str1, std::string str2, size_t len1, size_t len2);
        ~LongestCommonSubsequence();

        int length; // length of the longest common subsequence
        std::string substring;  // longest common character sequence

        int GetSubsequenceLength();
        std::string GetSubsequence();

    private:
        std::unique_ptr<std::unique_ptr<int[]>[]> dist;

};

LongestCommonSubsequence::LongestCommonSubsequence(std::string str1, std::string str2, size_t len1, size_t len2) {
    string1 = str1;
    string2 = str2;
    length1 = len1;
    length2 = len2;
}

LongestCommonSubsequence::~LongestCommonSubsequence() {
    if (dist != nullptr) {
        for(int i=0; i<length1; i++)
            dist[i].reset();
        dist.reset();
    }
}

int LongestCommonSubsequence::GetSubsequenceLength() {
    if (length1 == 0)
        return length2;
    else if (length2 == 0)
        return length1;

    int i, j, im, jm;
    size_t M = length1 + 1;
    size_t N = length2 + 1;

    // space allocation and initialization
    dist = std::make_unique<std::unique_ptr<int[]>[]>(M);
    for (i=0; i<M; i++) {
        dist[i] = std::make_unique<int[]>(N);
        for (j=0; j<N; j++) {
            dist[i][j] = 0;
        }
    }

    // longest common subsequence algorithm (finding length)
    for (j=1; j<N; j++) {
        jm = j - 1;
        for (i=1; i<M; i++) {
            im = i - 1;
            if (string1[im] == string2[jm]) {
                dist[i][j] = dist[im][jm] + 1;
            } else {
                dist[i][j] = std::max(dist[i][jm], dist[im][j]);
            }
        }
    }
    length = dist[length1][length2];
    return length;
}

std::string LongestCommonSubsequence::GetSubsequence() {
    int i, j;
    std::string default_message = "Firstly calculate length of subsequence!";
    if (dist == nullptr) {
        return default_message;
    }
    // finding sequence of characters
    i = length1;
    j = length2;
    while (i > 0 && j > 0) {
        if (string1[i-1] == string2[j-1]) {
            substring += string1[i-1];
            i--;
            j--;
        }
        else if (dist[i-1][j] > dist[i][j-1]) {
            i--;
        }
        else {
            j--;
        }
    }
    std::reverse(substring.begin(), substring.end());
    return substring; 
}

int main() {

    std::string str1, str2;
    size_t str1_len, str2_len;

    int longest_common_subsequence_len;
    std::string subsequence_string;

    std::cout << "Enter first string: "; 
    std::getline(std::cin, str1);
    str1_len = str1.length();

    std::cout << "Enter second string: ";
    std::getline(std::cin, str2);
    str2_len = str2.length();

    LongestCommonSubsequence subsequenceObj(str1, str2, str1_len, str2_len);
    longest_common_subsequence_len = subsequenceObj.GetSubsequenceLength();
    subsequence_string = subsequenceObj.GetSubsequence();

    std::cout << "Length of the longest common subsequence: " << longest_common_subsequence_len << std::endl;
    std::cout << "Subsequence: " << subsequence_string << std::endl;

    return EXIT_SUCCESS;
}