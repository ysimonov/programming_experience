/*
Semaphore is available in boost library
Also, available since c++20
std::counting_semaphore
std::binary_semaphore
*/

#include <chrono>
#include <condition_variable>
#include <mutex>
#include <thread>
#include <vector>

class Semaphore {
    unsigned long count_;
    std::mutex mutex_;
    std::condition_variable cv_;

   public:
    // the constructor
    // raises Runtime error is init count is zero
    Semaphore(unsigned long init_count) : count_(init_count) {
        if (init_count == 0) {
            throw std::runtime_error(
                "Zero value provided to semaphore constructor");
        }
    };

    // decrement the internal counter of the Semaphore
    void acquire() {
        std::unique_lock<std::mutex> lock(mutex_);
        while (!count_) {
            cv_.wait(lock);
        }
        if (count_ > 0) {
            count_--;
        }
    }

    // increment the internal counter of the Semaphore
    void release() {
        std::unique_lock<std::mutex> lock(mutex_);
        count_++;
        lock.unlock();
        cv_.notify_one();
    }
};

// charger supports the number of simultaneous
// charging points at once
// if n > 1 - counting semaphore
// if n = 1 - binary semaphore
Semaphore charger(3);

void cell_phone(int id) {
    // acquire the charger
    charger.acquire();
    printf("Phone %d is charging ...\n", id);
    // charge random amount
    srand(id);
    std::this_thread::sleep_for(
        std::chrono::milliseconds(rand() % 2000 + 1000));
    printf("Phone %d has finished charging!\n", id);
    charger.release();
}

int main() {
    std::vector<std::thread> phones;
    int num_phones = 10;

    phones.reserve(num_phones);
    for (int i = 0; i < num_phones; ++i) {
        phones.emplace_back(std::move(std::thread(cell_phone, i)));
    }

    for (auto& phone : phones) {
        phone.join();
    }
    return 0;
}