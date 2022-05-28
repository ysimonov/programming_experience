#include <iostream>
#include <string>
#include <algorithm>
#include <memory>
#include <vector>
#include <cstring>
#include <tuple>
#include <chrono>

/*
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


class RatcliffObershelp {

    std::string string1;
    std::string string2;
    int length1;
    int length2;

    public:

        RatcliffObershelp(std::string str1, std::string str2, size_t len1, size_t len2);
        ~RatcliffObershelp();

        int length = 0; // length of the longest common Substring

        std::tuple<int, int, int, int> LongestCommonSubstring(std::string, std::string);
        int GetMatchingLength(std::string, std::string);
        double GetSimilarityRatio();

    private:
        int right_row_anchor;
        int right_col_anchor;
};

RatcliffObershelp::RatcliffObershelp(std::string str1, std::string str2, size_t len1, size_t len2) {
    string1 = str1;
    string2 = str2;
    length1 = len1;
    length2 = len2;
}

RatcliffObershelp::~RatcliffObershelp() {}

std::tuple<int, int, int, int> RatcliffObershelp::LongestCommonSubstring(std::string str1, std::string str2) {

    size_t len1 = str1.length();
    size_t len2 = str2.length();
    if ((len1 == 0) || (len2 == 0))
        return std::make_tuple(-1, -1, -1, -1);

    std::unique_ptr<std::unique_ptr<int[]>[]> dist;
    size_t M = len1 + 1;
    size_t N = len2 + 1;
    int i, j, im, jm;
    int left_row_anchor = -1; 
    int left_col_anchor = -1;
    int right_row_anchor = -1;
    int right_col_anchor = -1;
    int len = 0;
    
    dist = std::make_unique<std::unique_ptr<int[]>[]>(M);
    for (i=0; i<M; i++) {
        dist[i] = std::make_unique<int[]>(N);
        for (j=0; j<N; j++) {
            dist[i][j] = 0;
        }
    }

    for (j=1; j<N; j++) {
        jm = j - 1;
        for (i=1; i<M; i++) {
            im = i - 1;
            if (str1[im] == str2[jm]) {
                dist[i][j] = dist[im][jm] + 1;
                if (len < dist[i][j]) {
                    len = dist[i][j];
                    right_row_anchor = i;
                    right_col_anchor = j;
                }
            }
        }
    }

    /* To print matching string */
    // if (len > 0) {
    //     int row = right_row_anchor;
    //     int col = right_col_anchor;
    //     std::string substring;
    //     while (dist[row][col] != 0) {
    //         substring += str1[row-1];
    //         row--;
    //         col--;
    //     }
    //     std::reverse(substring.begin(), substring.end());
    //     std::cout << "Matching String = " << substring << std::endl;
    // }

    left_row_anchor = right_row_anchor - len;
    left_col_anchor = right_col_anchor - len;

    return std::make_tuple(left_row_anchor, left_col_anchor, right_row_anchor, right_col_anchor);
}

int RatcliffObershelp::GetMatchingLength(std::string str1, std::string str2) {
    auto anchor_points = this->LongestCommonSubstring(str1, str2);
    int left_row_anchor = std::get<0>(anchor_points);
    int left_col_anchor = std::get<1>(anchor_points);
    int right_row_anchor = std::get<2>(anchor_points);
    int right_col_anchor = std::get<3>(anchor_points);

    if (left_row_anchor == right_row_anchor) {
        return 0;
    } else {
        int left_len = 0;
        if (left_row_anchor > 0 && left_col_anchor > 0) {
            // std::cout << "Left: " << std::endl;
            // std::cout << str1.substr(0, left_row_anchor) << std::endl;
            // std::cout << str2.substr(0, left_col_anchor) << std::endl;
            left_len = this->GetMatchingLength(
                str1.substr(0, left_row_anchor), 
                str2.substr(0, left_col_anchor));
        }

        int right_len = 0;
        if ((right_row_anchor > 0) && (right_col_anchor > 0)) {
            // std::cout << "Right: " << std::endl;
            // std::cout << str1.substr(right_row_anchor, str1.length()-1) << std::endl;
            // std::cout << str2.substr(right_col_anchor, str2.length()-1) << std::endl;
            right_len = this->GetMatchingLength(
                str1.substr(right_row_anchor, str1.length()-1), 
                str2.substr(right_col_anchor, str2.length()-1));
        }

        return left_len + (right_row_anchor - left_row_anchor) + right_len;
    }
}

double RatcliffObershelp::GetSimilarityRatio() {
    if((length1 == 0) || (length2 == 0))
        return 0.0;
    auto length = this->GetMatchingLength(string1, string2);
    return 2.0 * length / (length1 + length2);
}

double SimilarityRatio(std::string str1, std::string str2, size_t str1_len, size_t str2_len) {
    RatcliffObershelp substringObj(str1, str2, str1_len, str2_len);
    auto similarity_ratio = substringObj.GetSimilarityRatio();
    return similarity_ratio;
}

int main() {

    std::string str1, str2;
    size_t str1_len, str2_len;

    int longest_common_substring_len;
    std::string substring_string;
    double similarity_ratio;

    std::cout << "Enter first string: "; 
    std::getline(std::cin, str1);
    str1_len = str1.length();

    std::cout << "Enter second string: ";
    std::getline(std::cin, str2);
    str2_len = str2.length();

    RatcliffObershelp substringObj(str1, str2, str1_len, str2_len);
    similarity_ratio = substringObj.GetSimilarityRatio();
    std::cout << "Similarity ratio: " << similarity_ratio << std::endl;

    auto begin = std::chrono::high_resolution_clock::now();
    for (int i=0; i<100000; i++) {
        similarity_ratio = SimilarityRatio(str1, str2, str1_len, str2_len);
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << double(std::chrono::duration_cast<std::chrono::nanoseconds>(end-begin).count())/(1e9) << "s" << std::endl;

    return EXIT_SUCCESS;
}