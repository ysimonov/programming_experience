#include <iostream>
#include <string>
#include <map>
#include <algorithm>
#include <chrono>
#include <memory>

/*
    This algorithm implements True Damerau-Levenshtein distance between two strings, using reference from
    https://en.wikipedia.org/wiki/Damerau%E2%80%93Levenshtein_distance

    For example, 
    distance("banana", "abnana") = 1
    distance("anana", "banana") = 1
*/

inline int min(int x, int y) {
    if (x < y)
        return x;
    else 
        return y;
}

int DamerauLevenshtein(std::string str1, std::string str2) {
    size_t str1_len = str1.length();
    size_t str2_len = str2.length();
    if (str1_len == 0) {
        return str2_len;
    } 
    else if (str2_len == 0) {
        return str1_len;
    }
    size_t M = str1_len + 1;
    size_t N = str2_len + 1;
    size_t Pm = M + 1;
    size_t Pn = N + 1;
    size_t P = Pm * Pn;
    char ch1, ch2;

    std::map<char, int> da = {};

    int maxdist = str1_len + str2_len;
    int i, j, k, l, im, jm, ip, jp;
    int sub_cost;
    int substitution, insertion, deletion, transposition;
    int db;
    int levdist;

    int* dist = nullptr;    
    dist = new int[P];   

    // std::unique_ptr<int[]> dist = std::make_unique<int[]>(P);
    for (i=0; i<P; i++) {
        dist[i] = 0;
    }
    dist[0] = maxdist;
    for (i=0; i<M; i++) {
        dist[i*Pn] = maxdist;
        dist[(i+1)*Pn+1] = i;
    }
    dist[M*Pn] = maxdist;

    for (j=0; j<N; j++) {
        dist[j] = maxdist;
        dist[Pn+(j+1)] = j;
    }
    dist[Pn+N] = maxdist;

    for (i=0; i<str1_len; i++) {
        da[str1[i]] = 0;
    }
    for (j=0; j<str2_len; j++) {
        da[str2[j]] = 0;
    }
    for (i=1; i<M; i++) {
        im = i - 1;
        ip = i + 1;
        // Column of last match on this row: `DB` in pseudocode
        db = 0;
        for (j=1; j<N; j++) {
            jm = j - 1;
            jp = j + 1;
            k = da[str2[jm]];
            l = db;
            if (str1[im] == str2[jm]) {
                sub_cost = 0;
                db = j;
            } else {
                sub_cost = 1;
            }
            dist[ip*Pn+jp] = min(min(min(
                dist[i*Pn+j] + sub_cost, 
                dist[ip*Pn+j]+1), 
                dist[i*Pn+jp]+1), 
                dist[k*Pn+l] + (i-k-1) + 1 + (j-l-1));
        }
        // Update last row for current char
        da[str1[im]] = i;
    }

    levdist = dist[M*Pn+N];

    delete[] dist;
    dist = nullptr;

    return levdist;
}

int main() {

    int num_iterations = 1e6;

    std::string str1, str2;
    size_t str1_len, str2_len;

    int distance;
    std::string substring_string;
    double similarity_ratio;

    std::cout << "Enter first string: "; 
    std::getline(std::cin, str1);
    str1_len = str1.length();

    std::cout << "Enter second string: ";
    std::getline(std::cin, str2);
    str2_len = str2.length();

    auto begin = std::chrono::high_resolution_clock::now();
    for (int i=0; i<num_iterations; i++) {
        distance = DamerauLevenshtein(str1, str2);
        // std::cout << "Distance: " << distance << std::endl;
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << double(std::chrono::duration_cast<std::chrono::nanoseconds>(end-begin).count())/(1e9) << "s" << std::endl;

    return EXIT_SUCCESS;
}