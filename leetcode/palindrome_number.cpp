#include <cstdlib>
#include <iostream>

class Solution {
   public:
    bool isPalindrome(int x) {
        // any negative number cannot be palindrome
        if (x < 0) {
            return false;
        } else if (x < 10) {
            return true;
        }

        // cannot be palindrome if positive and divisible by 10 (ends with 0)
        // note this is valid because negative numbers are filtered out
        // and all numbers are starting from 10 onwards
        if (x % 10 == 0) {
            return false;
        }

        // palindrome numbers must satisfy relation a_(i) = a_(k-i)
        // construct a reverse digit
        unsigned int reverse{0};
        int n{x};
        while (n != 0) {
            reverse = reverse * 10 + n % 10;
            n /= 10;  // this moves one position to the left for digit x
        }

        if (reverse == x) {
            return true;
        }

        return false;
    }
};

int main() {
    Solution sol;

    int num;
    std::cout << "Enter number to check palindrome: ";
    std::cin >> num;

    auto res = sol.isPalindrome(num);

    if (res) {
        std::cout << "Number " << num << " is palindrome" << std::endl;
    } else {
        std::cout << "Number " << num << " is not palindrome" << std::endl;
    }

    return EXIT_SUCCESS;
}