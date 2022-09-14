#include <iostream>
#include <mutex>
#include <thread>

unsigned int garlic_count = 0;
unsigned int potato_count = 0;

/*
Recursive mutex is used to prevent deadlock from happening.
In the example below, function add_potato_and_garlic increments
potato_count while pensil lock is acquired. At the same time,
add_garlic function tries to lock pensil again, thus forming
a deadlock. Without recursive_mutex, and using simple mutex,
the program will freeze, and threads will wait forever until the lock
is released.
*/

std::recursive_mutex pensil;

void add_garlic() {
    pensil.lock();
    garlic_count++;
    pensil.unlock();
}

void add_potato_and_garlic() {
    pensil.lock();
    potato_count++;
    add_garlic();
    pensil.unlock();
}

void add_shopping_list() {
    for (int i = 0; i < 10000; i++) {
        add_potato_and_garlic();
    }
}

int main() {
    std::thread person1(&add_shopping_list);
    std::thread person2(&add_shopping_list);

    person1.join();
    person2.join();

    std::cout << "We should buy " << garlic_count << " garlics\n";
    std::cout << "We should buy " << potato_count << " potatoes\n";

    return 0;
}