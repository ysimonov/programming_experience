#include <iostream>
#include <mutex>
#include <string>
#include <thread>

// ensure locks are always taken in the same order by every thread
// another solution is to use the scoped lock, which takes ownership
// of the selected mutex

unsigned int sushi_count = 40;

void philosopher(std::mutex& chopsticks, std::string philosopher_name) {
    while (sushi_count > 0) {
        {
            std::scoped_lock lock(chopsticks);
            if (sushi_count) {
                std::cout << "Philosopher " << philosopher_name
                          << " is eating sushi.\n";
                sushi_count--;
            }
        }
        // here one of the philosophers leaves prematurely
        // leaving without releasing lock
        // however, since scoped locked operates within a particular scope,
        // the lock will be released when break is called
        if (sushi_count == 10) {
            std::cout << "Philosopher " << philosopher_name
                      << " has had enough sushi.\n";
            break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }
}

int main() {
    std::mutex chopsticks;

    // this creates a potential for deadlock
    // since if philosopher 1 acquires lock using chopstick a
    // it also requires chopstick b to acquire lock
    std::thread philosopher1(philosopher, std::ref(chopsticks), "One");
    std::thread philosopher2(philosopher, std::ref(chopsticks), "Two");

    philosopher1.join();
    philosopher2.join();
    printf("The philosophers are done eating food sushi.\n");

    return 0;
}