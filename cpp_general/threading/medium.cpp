#include <chrono>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <queue>
#include <string>
#include <thread>
#include <vector>

template <typename Data>
class Queue {
   private:
    std::queue<Data> queue_;
    mutable std::mutex mutex_;
    std::condition_variable condition_variable_;

   public:
    void wait() {
        std::unique_lock<std::mutex> lock(mutex_);
        {
            condition_variable_.wait(lock);
        }
    }

    void push(const Data &data) {
        std::unique_lock<std::mutex> lock(mutex_);

        queue_.push(data);
        // notify one comes after the data is pushed to the shared queue
        // this avoids waiting thread of being notified if the call to push
        // throws an exception

        lock.unlock();  // unlock the mutex

        condition_variable_.notify_one();
    }

    bool try_pop(Data &popped_value) {
        std::lock_guard<std::mutex> lock(mutex_);

        if (queue_.empty()) {
            return false;
        }

        popped_value = queue_.front();
        queue_.pop();
        return true;
    }

    void wait_and_pop(Data &popped_value) {
        std::unique_lock<std::mutex> lock(mutex_);
        while (queue_.empty()) {
            condition_variable_.wait(lock);
        }
        popped_value = queue_.front();
        queue_.pop();
    }

    bool empty() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return queue_.empty();
    }

    Data &front() {
        std::lock_guard<std::mutex> lock(mutex_);
        return queue_.front();
    }

    Data const &front() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return queue_.front();
    }

    void pop() {
        std::lock_guard<std::mutex> lock(mutex_);
        queue_.pop();
    }
};

template <typename Data>
void run_tasks(Queue<Data> &queue) {
    std::thread::id thread_id = std::this_thread::get_id();

    while (!queue.empty()) {
        std::string proxy;
        {
            proxy = queue.front();
            queue.pop();
        }
        {
            std::cout << "Thread " << thread_id << " gets data " << proxy << "\n\n";
        }

        // sleep for one second
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
}

int main() {
    Queue<std::string> queue;

    std::vector<std::string> data{"2561033035", "0980870098", "2039811994", "8258410235", "8454513599", "2446827749",
                                  "0517369040", "6333228275", "2878050600", "6547222572", "3334224258", "7458594884",
                                  "7631170452", "4097816537", "4555183378", "2827215711", "2701131490", "9652503437",
                                  "1262178254", "5229712067", "8649355391", "3634975519", "2976561557", "4631284221",
                                  "6239834064", "7003835068", "3478530388", "4979001579", "9085912268", "5031440393"};

    for (const auto &el : data) {
        queue.push(el);
    }

    std::vector<std::thread> threads;
    uint16_t num_threads = 4;
    for (int i = 0; i < num_threads; i++) {
        threads.push_back(std::thread(run_tasks<std::string>, std::ref(queue)));
    }

    for (auto &th : threads) {
        th.join();
    }
    return 0;
}