#include <unordered_set>
#include <vector>

using namespace std;

struct VectorHash {
    size_t operator()(const std::vector<int>& v) const {
        hash<int> hasher;
        size_t seed = 0;
        for (int i : v) {
            seed ^= hasher(i) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        }
        return seed;
    }
};

class Solution {
   public:
    template <typename T>
    void swap(T& t1, T& t2) {
        T temp = move(t1);
        t1 = move(t2);
        t2 = move(temp);
    }

    void generatePermutations(int k, vector<int>& permutation,
                              unordered_set<vector<int>, VectorHash>& dict) {
        if (k == 1) {
            // check if unique permutation
            dict.insert(permutation);
            return;
        } else {
            // initially k = length(permutation)
            generatePermutations(k - 1, permutation, dict);

            // generate permutations for k-th swapped with each k-1 initial
            for (int i = 0; i < k - 1; ++i) {
                if (k % 2 == 0) {
                    swap(permutation[i], permutation[k - 1]);
                } else {
                    swap(permutation[0], permutation[k - 1]);
                }
                generatePermutations(k - 1, permutation, dict);
            }
        }
    }

    vector<vector<int>> permuteUnique(vector<int>& nums) {
        unordered_set<vector<int>, VectorHash> dict;

        int n = nums.size();
        vector<int> permutation = nums;

        generatePermutations(n, permutation, dict);

        vector<vector<int>> permutations(dict.begin(), dict.end());
        return permutations;
    }
};