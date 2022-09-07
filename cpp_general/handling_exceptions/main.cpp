#include <iostream>
#include <limits>
#include <stdexcept>

void MyFunc(int c) {
    // throw exception if char is greater than max char
    if (c > std::numeric_limits<char>::max())
        throw std::invalid_argument("MyFunc argument too large.");
}

float Division(float num, float den) {
    // if denominator is zero,
    // throw runtime exception
    if (std::abs(den) < std::numeric_limits<float>::epsilon())
        throw std::runtime_error("Math error: Attempted to divide by zero.");

    return num / den;
}

int main() {
    try {
        MyFunc(256);
    } catch (std::invalid_argument &e) {
        std::cerr << e.what() << std::endl;
    }

    try {
        float res = Division(12.0, 0.0);
    } catch (const std::exception &e) {  // default exception handling
        std::cerr << e.what() << std::endl;
    }

    return EXIT_SUCCESS;
}