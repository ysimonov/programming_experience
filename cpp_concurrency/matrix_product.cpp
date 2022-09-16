/**
 * Challenge: Multiply two matrices
 */
#include <boost/asio.hpp>
#include <cmath>
#include <cstdio>
#include <iostream>
#include <mutex>
#include <new>  // nothrow
#include <stdexcept>
#include <string>
#include <thread>

std::recursive_mutex mutex_;

// g++ matrix_product.cpp -lboost_thread -lboost_system

/* sequential implementation of matrix multiply */
template <typename T>
void sequential_matrix_multiply(T ***A, size_t num_rows_a, size_t num_cols_a,
                                T ***B, size_t num_rows_b, size_t num_cols_b,
                                T ***C) {
    for (size_t i = 0; i < num_rows_a; ++i) {
        for (size_t j = 0; j < num_cols_b; ++j) {
            (*C)[i][j] = 0;  // initialize result cell to zero
            for (size_t k = 0; k < num_cols_a; ++k) {
                (*C)[i][j] += (*A)[i][k] * (*B)[k][j];
            }
        }
    }
}

/* helper function for parallel implementation of matrix multiplication */
template <typename T>
void quick_sum(T *Aik, T *Bkj, T *Cij) {
    // critical section
    std::unique_lock<std::recursive_mutex> lock(mutex_);
    (*Cij) += (*Aik) * (*Bkj);
}

/* parallel implementation of matrix multiply */
template <typename T>
void parallel_matrix_multiply(T ***A, size_t num_rows_a, size_t num_cols_a,
                              T ***B, size_t num_rows_b, size_t num_cols_b,
                              T ***C) {
    boost::asio::thread_pool pool(std::thread::hardware_concurrency());
    for (size_t i = 0; i < num_rows_a; ++i) {
        for (size_t j = 0; j < num_cols_b; ++j) {
            (*C)[i][j] = 0;  // initialize result cell to zero
            // TODO partition matrix product into chunks
            // Cij = SUM OVER k(Aik * Bkj)
            // Cijk = Aik * Bkj becomes a tensor product
            // Then contract tensor Cij = SUM OVER k(Cijk) ???
            // Not really space efficient ...
        }
    }
}

// allocates rows and cols in a matrix
// throws runtime exception if invalid rows or cols is provided,
// or if allocation space is insufficient to hold all matrix enties
template <typename T>
void allocate_matrix(T ***mat, const size_t rows, const size_t cols) {
    if (rows == 0) {
        throw std::runtime_error("Matrix has invalid rows of size 0!");
    }
    if (cols == 0) {
        throw std::runtime_error("Matrix has invalid cols of size 0!");
    }
    (*mat) = new (std::nothrow) T *[rows];
    if ((*mat) == nullptr) {
        throw std::runtime_error(
            "Could not allocate space for matrix with dimensions " +
            std::to_string(rows) + " x " + std::to_string(cols));
    } else {
        for (size_t i = 0; i < rows; ++i) {
            (*mat)[i] = new (std::nothrow) T[cols];
            if ((*mat)[i] == nullptr) {
                // deallocate all elements up until i
                for (size_t j = 0; j < i; ++j) {
                    delete[](*mat)[j];
                    (*mat)[j] = nullptr;
                }
                delete[](*mat);
                (*mat) = nullptr;
                throw std::runtime_error(
                    "Could not allocate space for matrix with dimensions " +
                    std::to_string(rows) + " x " + std::to_string(cols));
            }
        }
    }
}

// dellocate matrix
template <typename T>
void deallocate_matrix(T ***mat, size_t rows, size_t cols) {
    for (size_t i = 0; i < rows; ++i) {
        delete[](*mat)[i];
        (*mat)[i] = nullptr;
    }
    delete[](*mat);
    (*mat) = nullptr;
}

// intialize matrix with values in range 1 to 100
template <typename T>
void populate_matrix_with_random_values(T ***mat, size_t rows, size_t cols) {
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            (*mat)[i][j] = (T)(rand() % 100 + 1);
        }
    }
}

int main() {
    const int NUM_EVAL_RUNS = 3;
    const size_t NUM_ROWS_A = 1000;
    const size_t NUM_COLS_A = 1000;
    const size_t NUM_ROWS_B = NUM_COLS_A;
    const size_t NUM_COLS_B = 1000;

    // ***** General Allocations *****
    long **A = nullptr;
    try {
        allocate_matrix(&A, NUM_ROWS_A, NUM_COLS_A);
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    populate_matrix_with_random_values(&A, NUM_ROWS_A, NUM_COLS_A);

    long **B = nullptr;
    try {
        allocate_matrix(&B, NUM_ROWS_B, NUM_COLS_B);
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        deallocate_matrix(&A, NUM_ROWS_A, NUM_COLS_A);
        return EXIT_FAILURE;
    }
    populate_matrix_with_random_values(&B, NUM_ROWS_B, NUM_COLS_B);

    long **sequential_result = nullptr;
    try {
        allocate_matrix(&sequential_result, NUM_ROWS_A, NUM_COLS_B);
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        deallocate_matrix(&A, NUM_ROWS_A, NUM_COLS_A);
        deallocate_matrix(&B, NUM_ROWS_B, NUM_COLS_B);
        return EXIT_FAILURE;
    }

    long **parallel_result = nullptr;
    try {
        allocate_matrix(&parallel_result, NUM_ROWS_A, NUM_COLS_B);
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        deallocate_matrix(&A, NUM_ROWS_A, NUM_COLS_A);
        deallocate_matrix(&B, NUM_ROWS_B, NUM_COLS_B);
        deallocate_matrix(&sequential_result, NUM_ROWS_A, NUM_COLS_B);
        return EXIT_FAILURE;
    }

    // ***** Computation *****
    printf("Evaluating Sequential Implementation...\n");
    std::chrono::duration<double> sequential_time(0);
    sequential_matrix_multiply(&A, NUM_ROWS_A, NUM_COLS_A, &B, NUM_ROWS_B,
                               NUM_COLS_B, &sequential_result);  // "warm up"
    for (int i = 0; i < NUM_EVAL_RUNS; i++) {
        auto startTime = std::chrono::high_resolution_clock::now();
        sequential_matrix_multiply(&A, NUM_ROWS_A, NUM_COLS_A, &B, NUM_ROWS_B,
                                   NUM_COLS_B, &sequential_result);
        sequential_time +=
            std::chrono::high_resolution_clock::now() - startTime;
    }
    sequential_time /= NUM_EVAL_RUNS;

    printf("Evaluating Parallel Implementation...\n");
    std::chrono::duration<double> parallel_time(0);
    parallel_matrix_multiply(&A, NUM_ROWS_A, NUM_COLS_A, &B, NUM_ROWS_B,
                             NUM_COLS_B, &parallel_result);  // "warm up"
    for (int i = 0; i < NUM_EVAL_RUNS; i++) {
        auto startTime = std::chrono::high_resolution_clock::now();
        parallel_matrix_multiply(&A, NUM_ROWS_A, NUM_COLS_A, &B, NUM_ROWS_B,
                                 NUM_COLS_B, &parallel_result);
        parallel_time += std::chrono::high_resolution_clock::now() - startTime;
    }
    parallel_time /= NUM_EVAL_RUNS;

    // ***** VERIFICATION *****
    // verify sequential and parallel results
    for (size_t i = 0; i < NUM_ROWS_A; i++) {
        for (size_t j = 0; j < NUM_COLS_B; j++) {
            if (sequential_result[i][j] != parallel_result[i][j]) {
                printf("ERROR: Result mismatch at row %ld, col %ld!\n", i, j);
            }
        }
    }
    printf("Average Sequential Time: %.2f ms\n",
           sequential_time.count() * 1000);
    printf("  Average Parallel Time: %.2f ms\n", parallel_time.count() * 1000);
    printf("Speedup: %.2f\n", sequential_time / parallel_time);
    printf("Efficiency %.2f%%\n", 100 * (sequential_time / parallel_time) /
                                      std::thread::hardware_concurrency());

    // ***** General Deallocations *****
    deallocate_matrix(&A, NUM_ROWS_A, NUM_COLS_A);
    deallocate_matrix(&B, NUM_ROWS_B, NUM_COLS_B);
    deallocate_matrix(&sequential_result, NUM_ROWS_A, NUM_COLS_B);
    deallocate_matrix(&parallel_result, NUM_ROWS_A, NUM_COLS_B);

    return EXIT_SUCCESS;
}