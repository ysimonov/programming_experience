
#include <future>
#include <thread>

// g++ future.cpp -lboost_thread -lboost_system

int how_many_vegies() {
    printf("Person A is counting vegetables ...\n");
    std::this_thread::sleep_for(std::chrono::seconds(3));
    return 42;
}

int main() {
    printf("Person B asked Person A how many vegetables.\n");
    std::future<int> result = std::async(std::launch::async, how_many_vegies);
    printf(
        "Person B can do something else while waiting for the result from "
        "Person A.\n");

    // this part will be blocking
    printf("Person A has responded with %d.\n", result.get());

    return 0;
}