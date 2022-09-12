#include <cstdio>

// Not recommended in C++ (no type, simply used as string replacement)
// #define ONE 1
// #define HELLO "Hello, World!"

// constexpr is treated as actual constants
constexpr int ONE = 1;
constexpr const char* HELLO = "Hello, World!";

int main() {
    printf("ONE is %d\n", ONE);
    puts(HELLO);
    return 0;
}