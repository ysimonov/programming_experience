#include <iostream>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

// ensure locks are always taken in the same order by every thread
// another solution is to use the scoped lock, which takes ownership
// of the selected mutex

unsigned int sushi_count = 20000;

void philosopher(std::mutex& chopsticks, std::string philosopher_name) {
    unsigned int number_of_sushi_eaten_by_philosopher = 0;
    while (sushi_count > 0) {
        {
            std::scoped_lock lock(chopsticks);
            if (sushi_count) {
                std::cout << "Philosopher " << philosopher_name
                          << " is eating sushi." << std::endl;
                sushi_count--;
                number_of_sushi_eaten_by_philosopher += 1;
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    // this is to ensure that printouts occur sequentially
    {
        std::scoped_lock lock(chopsticks);
        std::cout << "Philosopher " << philosopher_name << " has eaten "
                  << number_of_sushi_eaten_by_philosopher << " sushi."
                  << std::endl;
    }
}

int main() {
    std::mutex chopsticks;

    std::vector<std::thread> philosophers(1000);

    // this creates a potential for deadlock
    // since if philosopher 1 acquires lock using chopstick a
    // it also requires chopstick b to acquire lock

    for (int i = 0; i < philosophers.size(); ++i) {
        philosophers[i] =
            std::thread(&philosopher, std::ref(chopsticks), std::to_string(i));
    }

    for (int i = 0; i < philosophers.size(); ++i) {
        philosophers[i].join();
    }

    printf("The philosophers are done eating food sushi.\n");

    return 0;
}