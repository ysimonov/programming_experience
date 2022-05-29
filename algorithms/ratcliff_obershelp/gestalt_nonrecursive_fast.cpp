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

int GetMatchingLength(std::string str1, std::string str2) {

    size_t len1 = str1.length();
    size_t len2 = str2.length();
    if ((len1 == 0) || (len2 == 0))
        return 0;

    size_t M = len1 + 1;
    size_t N = len2 + 1;
    size_t P = M * N;
    int i, j, k, im, jm;
    int left_str1_anchor = len1;
    int left_str2_anchor = len2;
    int right_str1_anchor = 0;
    int right_str2_anchor = 0;
    int right_str1_anchor_tmp = 0;
    int right_str2_anchor_tmp = 0;
    int match_len = 0;
    int match_len_prev = 0;
    int substr_len = 0;
    int * dist = nullptr;
    int temp;

    std::string str1_tmp, str2_tmp;

    do {
        match_len_prev = match_len;

        // Left Anchor
        if ((left_str1_anchor > 0) && (left_str2_anchor > 0)) {

            str1_tmp = str1.substr(0, left_str1_anchor);
            str2_tmp = str2.substr(0, left_str2_anchor);
            len1 = str1_tmp.length();
            len2 = str2_tmp.length();

            if ((len1 != 0) || (len2 != 0)) {

                // ***** START OF THE SUBSTRING SEARCHING BLOCK *****

                substr_len = 0;
                M = len1 + 1;
                N = len2 + 1;
                P = M * N;

                dist = new int[P];
                for (i=0; i<P; i++)
                    dist[i] = 0;
                
                for (i=1; i<M; i++) {
                    im = i - 1;
                    for (j=1; j<N; j++) {
                        jm = j - 1;
                        if (str1_tmp[im] == str2_tmp[jm]) {
                            temp = dist[im*N+jm] + 1;
                            dist[i*N+j] = temp;
                            if (substr_len < temp) {
                                substr_len = temp;
                                right_str1_anchor_tmp = i;
                                right_str2_anchor_tmp = j;
                            }
                        }
                    }
                }

                delete[] dist;
                dist = nullptr;

                // ***** END OF SUBSTRING MATCHING BLOCK *****

                if (substr_len > 0) {
                    right_str1_anchor = right_str1_anchor_tmp;
                    right_str2_anchor = right_str2_anchor_tmp;
                    left_str1_anchor = right_str1_anchor - substr_len;
                    left_str2_anchor = right_str2_anchor - substr_len;    
                    str1 = str1_tmp;
                    str2 = str2_tmp;
                    match_len += substr_len;
                }
            }
        }
    
        // Right Anchor
        if ((right_str1_anchor < str1.length()) && (right_str2_anchor < str2.length())) {

            str1_tmp = str1.substr(right_str1_anchor);
            str2_tmp = str2.substr(right_str2_anchor);
            len1 = str1_tmp.length();
            len2 = str2_tmp.length();

            if ((len1 != 0) && (len2 != 0)) {

                // ***** START OF THE SUBSTRING SEARCHING BLOCK *****

                substr_len = 0;
                M = len1 + 1;
                N = len2 + 1;
                P = M * N;

                dist = new int[P];
                for (i=0; i<P; i++)
                    dist[i] = 0;
                
                for (i=1; i<M; i++) {
                    im = i - 1;
                    for (j=1; j<N; j++) {
                        jm = j - 1;
                        if (str1_tmp[im] == str2_tmp[jm]) {
                            temp = dist[im*N+jm] + 1;
                            dist[i*N+j] = temp;
                            if (substr_len < temp) {
                                substr_len = temp;
                                right_str1_anchor_tmp = i;
                                right_str2_anchor_tmp = j;
                            }
                        }
                    }
                }

                delete[] dist;
                dist = nullptr;

                // ***** END OF SUBSTRING MATCHING BLOCK *****

                if (substr_len > 0) {
                    right_str1_anchor = right_str1_anchor_tmp;
                    right_str2_anchor = right_str2_anchor_tmp;
                    left_str1_anchor = right_str1_anchor - substr_len;
                    left_str2_anchor = right_str2_anchor - substr_len;    
                    str1 = str1_tmp;
                    str2 = str2_tmp;
                    match_len += substr_len;
                }
            }
        }
    } while (match_len != match_len_prev);

    if (dist != nullptr) {
        delete[] dist;
        dist = nullptr;
    }

    return match_len;
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

    int num_times_to_run = 1;

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
        std::cout << "Similarity Ratio: " << similarity_ratio << std::endl;
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << double(std::chrono::duration_cast<std::chrono::nanoseconds>(end-begin).count())/(1e9) << "s" << std::endl;

    return EXIT_SUCCESS;
}