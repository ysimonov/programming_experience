#include <iostream>
#include <mutex>
#include <string>
#include <thread>

// ensure locks are always taken in the same order by every thread
// another solution is to use the scoped lock, which takes ownership
// of the selected mutex

unsigned int sushi_count = 100;

void philosopher(std::mutex& first_chopstick, std::mutex& second_chopstick,
                 std::string philosopher_name) {
    while (sushi_count > 0) {
        // scoped lock will be created and destroyed once per iteration
        // however, to allow another philosopher to taste sushi, we need to
        // introduce sleep at the end of the lock's scope, so that another
        // philosopher can take ownership of the scoped lock
        {
            std::scoped_lock lock(first_chopstick, second_chopstick);
            if (sushi_count) {
                std::cout << "Philosopher " << philosopher_name
                          << " is eating sushi.\n";
                sushi_count--;
            }
        }  // this is the end of the scoped lock for current philosopher

        // here philosopher is dreaming about eating another sushi for 5
        // milliseconds :D
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }
}

int main() {
    std::mutex chopstick_a, chopstick_b;

    // this creates a potential for deadlock
    // since if philosopher 1 acquires lock using chopstick a
    // it also requires chopstick b to acquire lock
    std::thread philosopher1(philosopher, std::ref(chopstick_a),
                             std::ref(chopstick_b), "One");
    std::thread philosopher2(philosopher, std::ref(chopstick_b),
                             std::ref(chopstick_a), "Two");

    philosopher1.join();
    philosopher2.join();
    printf("The philosophers are done eating food sushi.\n");

    return 0;
}