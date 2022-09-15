/*
Two hungry threads trying to take soup
*/

#include <array>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <thread>

int soup_servings = 20;
std::mutex slow_cooker_lid;

// condition variable is used to lock the calling thread
// until notified to resume
std::condition_variable soup_taken;

void hungry_person(int id) {
    int put_lid_back = 0;
    while (soup_servings > 0) {
        // pick up the slow cooker lid to acquire mutex
        // unique lock guarantees unlock state on destruction,
        // which happens at the end of each iteration
        std::unique_lock<std::mutex> lid_lock(slow_cooker_lid);
        // if it is not your turn to get the serving,
        // and there are some servings left
        if ((id != soup_servings % 2) && (soup_servings > 0)) {
            put_lid_back++;
            // note that this wait operation automatically
            // releases the mutex and suspends the execution of the thread
            soup_taken.wait(lid_lock);
        }
        // if the thread checks the above condition,
        // and it is its turn to take soup from the cooker
        // then execution will continue past the while loop
        // take some soup from the slow cooker, if there is any soup left
        if (soup_servings > 0) {
            // take soup
            soup_servings--;
            // release lock on the lid
            lid_lock.unlock();
            // call the condition variable to signal
            // another hungry person (thread) that it is its turn
            // notify_one() will work if you don't consider what thread
            // should take turn, whereas notify_all() will let all threads
            // know that they need to wake up, which will check which one
            // should take next turn
            soup_taken.notify_all();
        }
    }
    printf("Person %d put the lid back %u times.\n", id, put_lid_back);
}

int main() {
    std::array<std::thread, 5> hungry_threads;

    // create a pool of hungry threads that
    // each take their turn to get a serving from the slow cooker
    for (int i = 0; i < hungry_threads.size(); i++) {
        hungry_threads[i] = std::thread(&hungry_person, i);
    }

    // join
    for (auto& th : hungry_threads) {
        // join function returns when the thread execution has completed
        if (th.joinable()) {
            th.join();
        }
    }

    return 0;
}