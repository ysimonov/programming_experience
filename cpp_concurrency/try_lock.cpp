#include <chrono>
#include <iostream>
#include <mutex>
#include <thread>

std::mutex pensil;
uint32_t items_on_notepad = 0;

void shopper(const char* name) {
    uint32_t items_to_add = 0;
    while (items_on_notepad < 40) {
        if (items_to_add && pensil.try_lock()) {
            items_on_notepad += items_to_add;
            std::cout << name << " added " << items_to_add
                      << " item(s) to notepad." << std::endl;
            items_to_add = 0;
            // spend time writing into notepad
            std::this_thread::sleep_for(std::chrono::milliseconds(300));
            // unlock the mutex and allow other thread to access the shared
            // memory region
            pensil.unlock();
        } else {  // lock for other things to buy
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            items_to_add++;
            std::cout << name << " found something else to buy." << std::endl;
        }
    }
}

int main() {
    auto start_time = std::chrono::steady_clock::now();
    std::thread person1(shopper, "Bob");
    std::thread person2(shopper, "Bill");
    person1.join();
    person2.join();
    auto elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(
                            std::chrono::steady_clock::now() - start_time)
                            .count();

    std::cout << "Elapsed time: " << (elapsed_time / 1000.0) << " seconds"
              << std::endl;
    return 0;
}