#include "matplotlibcpp.h"

namespace plt = matplotlibcpp;

// g++ test_matplotlib.cpp -std=c++11 -I/usr/include/python3.10 -lpython3.10

int main() {
    plt::plot({1, 3, 2, 4});
    plt::show();
}