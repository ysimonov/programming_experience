#include <atomic>
#include <chrono>
#include <condition_variable>
#include <future>
#include <iomanip>
#include <iostream>
#include <mutex>
#include <queue>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

std::atomic<bool> stop_flag;

template <typename Data>
class Queue {
   private:
    std::queue<Data> queue_;
    mutable std::mutex mutex_;
    std::condition_variable cond_;

   public:
    void push(const Data &&data) {
        {
            std::lock_guard<std::mutex> lock(mutex_);
            queue_.push(std::move(data));
        }
        cond_.notify_one();
    }

    bool try_pop(Data &data, const std::chrono::microseconds &timeout) {
        std::unique_lock<std::mutex> lock(mutex_);

        if (!cond_.wait_for(lock, timeout, [this] { return !queue_.empty(); })) {
            return false;
        }

        data = std::move(queue_.front());
        queue_.pop();

        return true;
    }
};

void read_and_print(Queue<std::string> &queue) {
    std::thread::id thread_id = std::this_thread::get_id();

    auto wait_duration = std::chrono::seconds(1);

    while (!stop_flag) {
        std::string new_data;
        auto data_acquired = queue.try_pop(new_data, wait_duration);

        if (data_acquired) {
            std::cout << ">> Thread " << thread_id << " received " << new_data << std::endl;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}

int main() {
    Queue<std::string> queue;

    auto max_concurrency = std::thread::hardware_concurrency();

    std::vector<std::thread> threads;

    for (int i = 0; i < max_concurrency; i++) {
        threads.emplace_back(std::thread(&read_and_print, std::ref(queue)));
    }

    for (int i = 0; i < 50; i++) {
        auto now = std::chrono::high_resolution_clock::now();
        auto time = std::chrono::system_clock::to_time_t(now);
        auto tm = *std::gmtime(&time);
        auto epoch = now.time_since_epoch();
        auto us = std::chrono::duration_cast<std::chrono::microseconds>(epoch).count() % 1000000;

        std::stringstream time_ss;
        time_ss << std::put_time(&tm, "%F %T.");

        std::stringstream timezone_ss;
        timezone_ss << std::put_time(&tm, " %Z\n");

        auto timestamp = time_ss.str() + std::to_string(us) + timezone_ss.str();

        // place data into the queue
        queue.push(std::move(timestamp));

        std::cout << "Thread " << std::this_thread::get_id() << " sent: " << timestamp << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(250));
    }

    stop_flag = true;
    for (auto &th : threads) {
        std::cout << "Thread " << th.get_id() << " joined main thread" << std::endl;
        th.join();
    }

    return EXIT_SUCCESS;
}
