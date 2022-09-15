#include <cstdio>
#include <future>

size_t recursive_sum(size_t low, size_t high, size_t depth = 0) {
    // check base case threshold
    if (depth > 3) {
        size_t sum = 0;
        for (auto i = low; i < high; ++i) {
            sum += i;
        }
        return sum;
    } else {  // parallel divide an conquer
        auto mid = low + (high - low) / 2;
        auto left_future =
            std::async(std::launch::async, recursive_sum, low, mid, depth + 1);
        auto right = recursive_sum(mid, high, depth + 1);
        auto left = left_future.get();
        return left + right;
    }
}

int main() {
    auto total = recursive_sum(0, 1000000000);

    printf("Result of addition is %ld\n", total);
    return 0;
}