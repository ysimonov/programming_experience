#include <chrono>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

// compillation: g++ -std=c++20 -static main.cpp -o main

std::mutex mu;

void threadFunction(int i) {
    std::lock_guard<std::mutex> lock(mu);
    std::cout << "Thread " << std::this_thread::get_id() << " received " << i << std::endl;
}

int main() {
    auto hardwareConcurrency = std::thread::hardware_concurrency();
    std::cout << "The number of concurrent threads is " << hardwareConcurrency << "\n";

    std::vector<std::thread> threads;

    for (int i = 0; i < hardwareConcurrency; i++) {
        std::thread thread(&threadFunction, i);
        threads.push_back(std::move(thread));
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    for (std::thread &thread : threads) {
        if (thread.joinable()) {
            thread.join();
        }
    }
    threads.clear();

    return EXIT_SUCCESS;
}