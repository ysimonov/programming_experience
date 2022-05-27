#include <iostream>
#include <string>
#include <algorithm>
#include <memory>
#include <vector>
#include <cstring>

int LongestCommonSubstring(std::string str1, std::string str2, size_t str1_len, size_t str2_len) {
    if (str1_len == 0) 
        return str2_len;
    else if (str2_len == 0)
        return str1_len;

    size_t M = str1_len + 1;
    size_t N = str2_len + 1;
    int i, j;
    int im, jm;
    int row, col;
    int len = 0;
    std::unique_ptr<std::unique_ptr<int[]>[]> dist;

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
                    row = i;
                    col = j;
                }
            }
        }
    }

    std::string substr;
    while (dist[row][col] != 0) {
        substr += str1[row-1];
        row--;
        col--;
    }
    std::reverse(substr.begin(), substr.end());

    std::cout << "Substring: " << substr << std::endl;

    return len;

}

int LongestCommonSubsequence(std::string str1, std::string str2, size_t str1_len, size_t str2_len) {
    if (str1_len == 0) 
        return str2_len;
    else if (str2_len == 0)
        return str1_len;

    size_t M = str1_len + 1;
    size_t N = str2_len + 1;
    int i, j;
    int im, jm;
    int row, col;
    int len = 0;
    std::unique_ptr<std::unique_ptr<int[]>[]> dist;

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
            } else {
                dist[i][j] = std::max(dist[i][jm], dist[im][j]);
            }
        }
    }

    std::string substr;
    i = str1_len;
    j = str2_len;
    while (i > 0 && j > 0) {
        if (str1[i-1] == str2[j-1]) {
            substr += str1[i-1];
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
    std::reverse(substr.begin(), substr.end());
    std::cout << "Substring: " << substr << std::endl;

    len = dist[str1_len][str2_len];
    return len;
}

double RatcliffObershelpStringSimilarityRatio(std::string str1, std::string str2, size_t str1_len, size_t str2_len) {
    
    if ((str1_len == 0) || (str2_len == 0))
        return 0.0;

    size_t M = str1_len + 1;
    size_t N = str2_len + 1;
    int i, j;
    int im, jm;
    int len = 0;
    double string_ratio = 0.0;
    std::unique_ptr<std::unique_ptr<int[]>[]> dist;

    dist = std::make_unique<std::unique_ptr<int[]>[]>(M);
    for (i=0; i<M; i++) {
        dist[i] = std::make_unique<int[]>(N);
        for (j=0; j<N; j++) {
            dist[i][j] = 0;
        }
    }

    int row_right = 0;
    int col_right = 0;

    // find longest common string
    for (j=1; j<N; j++) {
        jm = j - 1;
        for (i=1; i<M; i++) {
            im = i - 1;
            if (str1[im] == str2[jm]) {
                dist[i][j] = dist[im][jm] + 1;
                if (len < dist[i][j]) {
                    len = dist[i][j];
                    row_right = i;
                    col_right = j;
                }
            }
        }
    }

    // find anchor points
    std::string substr;
    
    int row_left = row_right;
    int col_left = col_right;
    int len_left = 0;
    int len_right = 0;

    if (len > 0) {
        while (dist[row_left][col_left] != 0) {
            substr += str1[row_left-1];
            row_left--;
            col_left--;
        }
        std::cout << "First character: " << str1[row_left] << " " << str2[col_left] << std::endl;
        std::cout << "Last character: " << str1[row_right-1] << " " << str2[col_right-1] << std::endl;
        std::reverse(substr.begin(), substr.end());
        std::cout << "Substring: " << substr << std::endl;

        // now find iteratively longest common sequence of strings to the left of the anchor
        for (j=0; j<col_left; j++){
            for (i=0; i<row_left; i++) {
                dist[i][j] = 0;
            }
        }
        for (j=1; j<col_left; j++) {
            jm = j - 1;
            for (i=1; i<row_left; i++) {
                im = i - 1;
                if (str1[im] == str2[jm]) {
                    dist[i][j] = dist[im][jm] + 1;
                    if (len_left < dist[i][j])
                        len_left = dist[i][j];
                }
            }
        }

        // now find iteratively longest common sequence of strings to the right of the anchor
        int j1 = std::max(col_right-1, 0);
        int i1 = std::max(row_right-1, 0);

        for (j=j1; j<N; j++){
            for (i=i1; i<M; i++) {
                dist[i][j] = 0;
            }
        }
        for (j=col_right; j<N; j++) {
            jm = j - 1;
            for (i=row_right; i<M; i++) {
                im = i - 1;
                if (str1[im] == str2[jm]) {
                    dist[i][j] = dist[im][jm] + 1;
                    if (len_right < dist[i][j])
                        len_right = dist[i][j];
                }
            }
        }      

    }

    std::cout << "len(left): " << len_left << std::endl;
    std::cout << "len(right): " << len_right << std::endl;

    string_ratio = 2.0 * std::max(len + len_left, len + len_right) / (str1_len + str2_len);
    return string_ratio;
}


int main() {

    std::string str1, str2;
    size_t str1_len, str2_len;

    double string_ratio;
    int longest_common_substring_len;
    int longest_common_subsequence_len;

    std::cout << "Enter first string: "; 
    std::getline(std::cin, str1);
    str1_len = str1.length();

    std::cout << "Enter second string: ";
    std::getline(std::cin, str2);
    str2_len = str2.length();

    string_ratio = RatcliffObershelpStringSimilarityRatio(str1, str2, str1_len, str2_len);
    std::cout << "Similarity ratio between\n\t" << str1 << "\nand\n\t" << str2 << "\nis " << string_ratio << std::endl;

    // longest_common_subsequence_len = LongestCommonSubsequence(str1, str2, str1_len, str2_len);
    // std::cout << "Length of the longest common subsequence is " << longest_common_subsequence_len << std::endl;

    // longest_common_substring_len = LongestCommonSubstring(str1, str2, str1_len, str2_len);
    // std::cout << "Length of the longest common substring is " << longest_common_substring_len << std::endl;

    return EXIT_SUCCESS;
}