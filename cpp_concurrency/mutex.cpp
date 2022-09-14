#include <atomic>
#include <chrono>
#include <iostream>
#include <mutex>
#include <thread>

// unsigned int garlic_count = 0;
std::mutex pensil;

void shopper(std::atomic<unsigned int>* garlic_count,
             const char* shopper_name) {
    for (int i = 0; i < 5; i++) {
        std::cout << "Shopper " << shopper_name << " is thinking ...\n";

        std::this_thread::sleep_for(std::chrono::milliseconds(500));

        pensil.lock();
        *garlic_count += 1;
        pensil.unlock();
    }
}

int main() {
    // can use atomic instead of global
    // note that atomic doesn't require lock
    // since information is synchronized across threads
    // this is just for demostration purposes
    std::atomic<unsigned int> garlic_count(0);

    std::thread barron(&shopper, &garlic_count, "Barron");
    std::thread olivia(&shopper, &garlic_count, "Olivia");

    barron.join();
    olivia.join();

    std::cout << "Total number of garlic(s): " << garlic_count.load()
              << std::endl;

    return 0;
}