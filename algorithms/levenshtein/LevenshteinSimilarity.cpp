#include "LevenshteinSimilarity.h"

#include <algorithm>
#include <iostream>
#include <memory>
#include <string>

/*
    The purpose of this program is to compute
    Similarity Ratio based on Levenshtein Distance
    between two utf-8 encoded strings.
    The function returns
    ratio = (lensum - levdist) / lensum if both strings
    are of non-zero length, else 0.0
*/

std::string inline lower_case(std::string str) {
    std::string str_lower = str;
    std::for_each(
        str_lower.begin(),
        str_lower.end(),
        [](char &c) {
            c = ::tolower(c);
        });
    return str_lower;
}

double levenshtein_ratio(std::string str1, std::string str2) {
    size_t M = str1.length();
    size_t N = str2.length();

    if ((M == 0) | (N == 0))
        return 0.0;

    std::string str1_lower = lower_case(str1);
    std::string str2_lower = lower_case(str2);

    size_t Mp = M + 1;
    size_t Np = N + 1;
    size_t lensum = M + N;

    double string_ratio = 0.0;

    std::unique_ptr<std::unique_ptr<int[]>[]> dist;
    std::unique_ptr<int[]> dist_flat;

    int levdist;
    int sub_cost;
    int i, j;
    int im, jm;

    dist = std::make_unique<std::unique_ptr<int[]>[]>(Mp);

    for (i = 0; i < Mp; i++) {
        dist_flat = std::make_unique<int[]>(Np);
        for (j = 0; j < Np; j++) {
            dist_flat[j] = 0;
        }
        dist[i] = std::move(dist_flat);
        dist[i][0] = i;
    }

    for (j = 1; j < Np; j++)
        dist[0][j] = j;

    for (j = 1; j < Np; j++) {
        jm = j - 1;
        for (i = 1; i < Mp; i++) {
            im = i - 1;
            if (str1_lower[im] == str2_lower[jm]) {
                sub_cost = 0;
            } else {
                sub_cost = 1;
            }
            dist[i][j] = std::min({dist[im][j] + 1,
                                   dist[i][jm] + 1,
                                   dist[im][jm] + sub_cost});
        }
    }

    levdist = dist[M][N];

    // std::cout << "Levdist: " << levdist << std::endl;

    string_ratio = double(lensum - levdist) / lensum;

    return string_ratio;
}

int main() {
    std::string str1;
    std::string str2;
    double string_ratio;

    std::cout << "Enter first string: ";
    std::getline(std::cin, str1);

    std::cout << "Enter second string: ";
    std::getline(std::cin, str2);

    string_ratio = levenshtein_ratio(str1, str2);
    std::cout << "Similarity ratio between\n\t" << str1 << "\nand\n\t" << str2 << "\nis " << string_ratio << std::endl;

    return EXIT_SUCCESS;
}