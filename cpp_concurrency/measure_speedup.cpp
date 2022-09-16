#include <chrono>
#include <future>
#include <iostream>

// ***Throughput***
// is a measure of how many units of information a system can process in a given
// amount of time

// ***Latency***
// delay between invoking the operation and getting the response

// ***Amdahl's Law***
// Speedup(latency) = 1 / ((1 - p) + p / s)
// p - proportion of execution time that the part benefiting from
// improved resources originally occupied
// s - speedup of the part of the task that benefits from improved system
// resources

// ***Strong scaling***
// is defined as how the solution time varies with the number of processors for
// a fixed total problem size

// ***Weak scaling***
// is defined as how the solution time varies with the number of processors for
// a fixed problem size per processor

// ***Efficiency***
// how well the parallel processing resources are being utilized

const int NUM_EVAL_RUNS = 10;
const int SUM_VALUE = 100000000;

// basic sequential sum that adds up numbers between low and high inclusive
inline uintmax_t sequential_sum(uintmax_t low, uintmax_t high) {
    uintmax_t sum = 0;
    for (auto i = low; i < high; ++i) {
        sum += i;
    }
    return sum;
}

// recursive parallel sum pased on divide and conquor algorithm
// with a preset reqursion depth. The function calculates the sum
// of numbers between low and high inclusive. When recursion depth
// is exceeded, parallel sum is calculated as sequential sum
uintmax_t parallel_sum(uintmax_t low, uintmax_t high, uint8_t depth = 0U) {
    // check base case threshold
    if (depth > 3) {
        return sequential_sum(low, high);
    } else {  // parallel divide an conquer
        auto mid = low + (high - low) / 2;
        auto left_future =
            std::async(std::launch::async, parallel_sum, low, mid, depth + 1);
        auto right = parallel_sum(mid, high, depth + 1);
        auto left = left_future.get();
        return left + right;
    }
}

int main() {
    std::cout << "Calculation run time of sequential implementation... \n";
    std::chrono::duration<double> sequential_time(0);
    for (int i = 0; i < NUM_EVAL_RUNS; ++i) {
        auto start_time = std::chrono::high_resolution_clock::now();
        sequential_sum(0, SUM_VALUE);
        sequential_time +=
            std::chrono::high_resolution_clock::now() - start_time;
    }
    sequential_time /= NUM_EVAL_RUNS;

    std::cout << "Calculation run time of parallel implementation... \n";
    std::chrono::duration<double> parallel_time(0);
    for (int i = 0; i < NUM_EVAL_RUNS; ++i) {
        auto start_time = std::chrono::high_resolution_clock::now();
        parallel_sum(0, SUM_VALUE);
        parallel_time += std::chrono::high_resolution_clock::now() - start_time;
    }
    parallel_time /= NUM_EVAL_RUNS;

    printf("Average Sequential Time: %.1f ms\n",
           sequential_time.count() * 1000);
    printf("Average Parallel Time: %.1f ms\n", parallel_time.count() * 1000);
    printf("Speedup: %.2f\n", sequential_time / parallel_time);
    printf("Efficiency: %.2f%%\n", (sequential_time / parallel_time) * 100 /
                                       std::thread::hardware_concurrency());

    return 0;
}