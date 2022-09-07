#include <iostream>

class Solution {
   public:
    int countPrimes(int n) {
        // constrained
        // by 0 <= n
        // <= 5 *
        // 10^6
        if (n <= 2) {
            return 0;
        } else if (n == 3) {
            return 1;
        }

        int k = (n - 2) / 2;
        uintmax_t kmax = static_cast<uintmax_t>(k);

        int m = k + 1;
        uintmax_t mmax = static_cast<uintmax_t>(m);

        int num_primes = 1;

        bool *mask = new bool[m];
        for (int i = 0; i < m; ++i) {
            mask[i] = true;
        }

#pragma omp parallel for private(i, j) shared(k_max, mmax, mask)
        for (uintmax_t i = 1; i < mmax; ++i) {
            uintmax_t j = i;
            while (i + j + 2 * i * j <= kmax) {
                mask[i + j + 2 * i * j] = false;
                j += 1;
            }
        }

#pragma omp parallel for private(i) shared(m, mask) reduction(+ : num_primes)
        for (int i = 1; i < m; ++i) {
            if (mask[i]) {
                num_primes += 1;
            }
        }

        delete[] mask;
        mask = nullptr;

        return num_primes;
    }
};

int main() {
    Solution sol;

    auto n = 50000;
    auto res = sol.countPrimes(n);

    std::cout << "Counted " << res << " primes from " << n << " numbers"
              << std::endl;

    return EXIT_SUCCESS;
}