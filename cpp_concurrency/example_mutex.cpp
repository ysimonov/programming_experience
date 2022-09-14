#include <atomic>
#include <iostream>
#include <mutex>
#include <thread>

// unsigned int garlic_count = 0;
std::mutex pensil;

void shopper(std::atomic<unsigned int>* garlic_count) {
    pensil.lock();
    for (int i = 0; i < 1000000; i++) {
        *garlic_count += 1;
    }
    pensil.unlock();
}

int main() {
    // can use atomic instead of global
    std::atomic<unsigned int> garlic_count(0);

    std::thread barron(&shopper, &garlic_count);
    std::thread olivia(&shopper, &garlic_count);

    barron.join();
    olivia.join();

    std::cout << "Total number of garlic(s): " << garlic_count << std::endl;

    return 0;
}