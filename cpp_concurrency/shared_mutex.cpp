#include <atomic>
#include <chrono>
#include <iostream>
#include <shared_mutex>
#include <string>
#include <thread>

// Shared mutex only became available since C++17
// shared_mutex has methods called lock_shared() and unlock_shared()
// these methods allow reader threads to access shared resource for reading
// since reading operation does not result in the race condition

std::string WEEKDAYS[] = {"Monday", "Tuesday",  "Wednesday", "Thursday",
                          "Friday", "Saturday", "Sunday"};

std::atomic<int> today(0);
std::shared_timed_mutex marker;

void calendar_reader(const int id) {
    for (int i = 0; i < 20; i++) {
        marker.lock_shared();
        std::cout << "Reader " << id << " sees today is " << WEEKDAYS[today]
                  << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        marker.unlock_shared();
    }
}

void calendar_writer(const int id) {
    for (int i = 0; i < 20; i++) {
        marker.lock();
        today = (today + 1) % 7;
        std::cout << "Writer " << id << " updated date to " << WEEKDAYS[today]
                  << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        marker.unlock();
    }
}

int main() {
    std::array<std::thread, 10> readers;
    std::array<std::thread, 2> writers;

    // reader threads
    for (int i = 0; i < readers.size(); i++) {
        readers[i] = std::thread(calendar_reader, i);
    }

    // writer threads
    for (int i = 0; i < writers.size(); i++) {
        writers[i] = std::thread(calendar_writer, i);
    }

    // wait for readers and writers to finish their tasks
    for (auto& reader : readers) {
        if (reader.joinable()) {
            reader.join();
        }
    }

    for (auto& writer : writers) {
        if (writer.joinable()) {
            writer.join();
        }
    }

    return 0;
}