/*
Reusing n_threads in the pool to complete all n_tasks tasks
*/

#include <boost/asio.hpp>
#include <cstdio>
#include <iostream>
#include <mutex>
#include <thread>

std::mutex mut_;

// g++ thread_pool.cpp -lboost_thread -lboost_system

void vegetable_chopper(const int vegetable_id) {
    std::unique_lock<std::mutex> lock(mut_);
    std::cout << "Thread " << std::this_thread::get_id()
              << " chopped vegetable " << vegetable_id << std::endl;
}

int main() {
    int n_threads = 4;
    int n_tasks = 100;

    boost::asio::thread_pool pool(n_threads);

    // submitting lambda expression to post
    for (int i = 0; i < n_tasks; i++) {
        boost::asio::post(pool, [&i]() { vegetable_chopper(i); });
    }
    pool.join();

    return 0;
}