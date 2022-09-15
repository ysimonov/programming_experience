#include <boost/thread/latch.hpp>
#include <mutex>
#include <thread>

// g++ latch.cpp -lboost_system -lboost_date_time -lboost_thread

unsigned int bags_of_chips = 1;
std::mutex pencil;
boost::latch fist_bump(5);

void cpu_work(unsigned int work_units) {
    unsigned long x = 0;
    for (size_t i = 0; i < work_units; i++) {
        x++;
    }
}

void shopper_one() {
    cpu_work(1);
    fist_bump.wait();
    std::scoped_lock<std::mutex> lock(pencil);
    bags_of_chips *= 2;
    printf("Shopper ONE DOUBLED the number of bags of chips to buy.\n");
}

void shopper_two() {
    cpu_work(1);
    {
        // acquire lock
        std::scoped_lock<std::mutex> lock(pencil);
        // modify global bags_of_chips variable
        bags_of_chips += 3;
    }  // scoped lock goes out of scope
    printf("Shopper TWO ADDED 3 bags of chips to the shopping list.\n");
    // prevents threads that are waiting
    // until count reaches zero
    // therefore it is important to initialize the latch
    // with the appropriate value = number of times this thread should run
    fist_bump.count_down();
}

int main() {
    std::thread shoppers[10];
    for (int i = 0; i < 10; i += 2) {
        shoppers[i] = std::thread(shopper_one);
        shoppers[i + 1] = std::thread(shopper_two);
    }

    for (auto& shopper : shoppers) {
        shopper.join();
    }

    printf("The total number of chips to buy: %d\n", bags_of_chips);

    return 0;
}