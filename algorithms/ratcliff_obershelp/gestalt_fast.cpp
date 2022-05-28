#include <iostream>
#include <string>
#include <algorithm>
#include <memory>
#include <vector>
#include <cstring>
#include <tuple>
#include <array>
#include <chrono>

/*
    AUTHOR: Yevgeniy Simonov, 2022

    This algorithm uses is based on Gestalt pattern matching, between two strings.
    For more information about this algorithm, visit:
    https://en.wikipedia.org/wiki/Gestalt_Pattern_Matching

    The main idea behind the algorithm is finding longest common substring, then
    labelling start and end point of the string as anchor. The left anchor will be used
    for repeated longest commmon substring calculation to the left of the anchor,
    while the right anchor will be used for repeated longest common substring calculation 
    to the right of the anchor.

    The result of the algorithm is similarity ratio, that is described by:
    SIMILARITY RATIO = 2 * NUMBER OF MATCHING CHARACTERS / (LENGTH OF STRING 1 + LENGTH OF STRING 2)
    
    Let Km = NUMBER OF MATCHING CHARACTERS

    Basic steps:
    1. Find the longest substring that two strings have in common (Anchor)
    2. Km is increased by the number of matching characters (Anchor's length)
    3. The remaining parts of the string to the left and to the right of the anchor
       must be examined as if they were new strings (step 1 is repeated)
    4. Repeat step 3 until all the characters of string 1 and string 2 are analyzed

    Example:
    Morning How Are you?
    good morning how are you?

    1.) length of string 1 = 20
        length of string 2 = 25
    2.) The longest substring that they strigs have in common: "orning " => Km = 7
    3.) To the left of the anchor "orning " the are no more characters in common
    4.) Therefore, Km = Km + left = 7 + 0 = 7
    5.) To the right of the anchor the next longest substring is "re you?" => right = 7
    6.) Therefore, Km = Km + right = 7 + 7 = 14
    7.) Now the remainining part that hasn't been examined is 
        "How A" (string1) and "how a" (string2) => left from anchor "re you?" => left = 3
    8.) Therefore, Km = Km + left = 14 + 3 = 17
    9.) similarity_ratio = 2 * 17 / (20 + 25) = 0.75555...
    
    OVERALL Substring patters found during this algorithm + remaining strings:
    1.) "orning "  len(7)
        Remaining string1: "M" (left)   "How Are you?" (right)
        Remaining string2: "good m" (left)    "how are you?" (right)
    2.) No matching characters for the left anchor
        Remaining string1: "How Are you?" (right)
        Remaining string2: "how are you?" (right)
    3.) "re you?" len(7)
        Remaining string1: "How A"
        Remaining string2: "how a"
    4.) "ow " len(3)
        Remaining string1: "H" (left) "A" (right)
        Remaining string2: "h" (left) "a" (right)
    5.) No matching characters for the left anchor:
        Remaining string1: "A" (right)
        Remaining string2: "a" (right)
    6.) No matching characters for the right anchor.
        No remaining strings. => total length = 7 + 7 + 3 = 17
*/

inline std::array<int, 4> LongestCommonSubstring(std::string str1, std::string str2) {

    size_t len1 = str1.length();
    size_t len2 = str2.length();
    if ((len1 == 0) || (len2 == 0))
        return {-1, -1, -1, -1};

    size_t M = len1 + 1;
    size_t N = len2 + 1;
    int i, j, im, jm;
    int left_row_anchor = -1; 
    int left_col_anchor = -1;
    int right_row_anchor = -1;
    int right_col_anchor = -1;
    int len = 0;
    
    int * dist = nullptr;
    int P = M * N;
    int temp;

    dist = new int[P];
    for (i=0; i<P; i++) {
        dist[i] = 0;
    }

    for (j=1; j<N; j++) {
        jm = j - 1;
        for (i=1; i<M; i++) {
            im = i - 1;
            if (str1[im] == str2[jm]) {
                temp = dist[im*N+jm] + 1;
                dist[i*N+j] = temp;
                if (len < temp) {
                    len = temp;
                    right_row_anchor = i;
                    right_col_anchor = j;
                }
            }
        }
    }

    delete[] dist;
    dist = nullptr;

    left_row_anchor = right_row_anchor - len;
    left_col_anchor = right_col_anchor - len;

    return {left_row_anchor, left_col_anchor, right_row_anchor, right_col_anchor};
}

int GetMatchingLength(std::string str1, std::string str2) {
    auto anchor_points = LongestCommonSubstring(str1, str2);
    int left_row_anchor = anchor_points[0];
    int left_col_anchor = anchor_points[1];
    int right_row_anchor = anchor_points[2];
    int right_col_anchor = anchor_points[3];

    if (left_row_anchor == right_row_anchor) {
        return 0;
    } else {
        int left_len = 0;
        if ((left_row_anchor > 0) && (left_col_anchor > 0)) {
            left_len = GetMatchingLength(
                str1.substr(0, left_row_anchor), 
                str2.substr(0, left_col_anchor));
        }
        int right_len = 0;
        if ((right_row_anchor > 0) && (right_col_anchor > 0)) {
            right_len = GetMatchingLength(
                str1.substr(right_row_anchor), 
                str2.substr(right_col_anchor));
        }
        return left_len + (right_row_anchor - left_row_anchor) + right_len;
    }
}

double SimilarityRatio(std::string str1, std::string str2) {
    size_t str1_len = str1.length();
    size_t str2_len = str2.length();
    if ((str1_len == 0) || (str2_len == 0)) {
        return 0.0;
    }
    std::string str1_tmp = str1;
    std::string str2_tmp = str2;
    auto len = GetMatchingLength(str1_tmp, str2_tmp);
    return 2.0 * len / (str1_len + str2_len);
}

int main() {

    int num_times_to_run = 1e6;

    std::string str1, str2;
    size_t str1_len, str2_len;

    int longest_common_substring_len;
    std::string substring_string;
    double similarity_ratio;

    std::cout << "Enter first string: "; 
    std::getline(std::cin, str1);

    std::cout << "Enter second string: ";
    std::getline(std::cin, str2);

    auto begin = std::chrono::high_resolution_clock::now();
    for (int i=0; i<num_times_to_run; i++) {
        similarity_ratio = SimilarityRatio(str1, str2);
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << double(std::chrono::duration_cast<std::chrono::nanoseconds>(end-begin).count())/(1e9) << "s" << std::endl;

    return EXIT_SUCCESS;
}