/*
Thread serving and eating soup
*/

#include <array>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>

class ServingLine {
    std::queue<int> soup_queue;
    std::mutex ladle;
    std::condition_variable soup_served;

   public:
    void serve_soup(int i) {
        // since the queue is a shared object,
        // we need to lock access to this source to prevent race condition
        std::unique_lock<std::mutex> ladle_lock(ladle);
        // push data to the queue
        soup_queue.push(i);
        // release the lock and notify one of the consumer threads after
        ladle_lock.unlock();
        // notify one of the consumer threads
        soup_served.notify_one();
    }

    int take_soup() {
        // condition variable in C++ accepts only unique lock
        std::unique_lock<std::mutex> ladle_lock(ladle);
        while (soup_queue.empty()) {
            soup_served.wait(ladle_lock);
        }
        // returns a reference to the read data
        // at the first element in the queue
        int bowl = soup_queue.front();
        // removes first element from the queue
        // and shrinks queue size by one
        soup_queue.pop();
        return bowl;
    }
};

ServingLine serving_line = ServingLine();

void soup_producer() {
    int number_of_bowls_of_soup = 100000;
    for (int i = 0; i < number_of_bowls_of_soup; ++i) {
        serving_line.serve_soup(1);
    }
    // indicate that no more soup should be placed in the queue
    serving_line.serve_soup(-1);
    printf("Producer finished serving soup!\n");
}

void soup_consumer(const char* consumer_name) {
    int soup_eaten = 0;
    while (1) {
        int bowl = serving_line.take_soup();
        // check if it is the last portion of soup
        if (bowl == -1) {
            printf("Consumer %s ate %d bowls of soup.\n", consumer_name,
                   soup_eaten);
            // signal all other consumers to stop eating soup (IMPORTANT)
            // if this signal is not provided, consumers will be stuck on
            // waiting for their portion of soup (take_soup() function)
            serving_line.serve_soup(-1);
            return;
        } else {
            // eat the soup
            soup_eaten += bowl;
        }
    }
}

int main() {
    std::thread producer(&soup_producer);
    std::array<std::thread, 5> consumers;
    std::array<std::string, 5> consumer_names = {"Robert", "Gilbert", "Gerrard",
                                                 "Jerry", "Steven"};

    for (int i = 0; i < consumers.size(); ++i) {
        const auto& consumer_name = consumer_names[i];
        consumers[i] = std::thread(&soup_consumer, consumer_name.c_str());
    }

    producer.join();
    for (auto& consumer : consumers) {
        consumer.join();
    }

    return 0;
}