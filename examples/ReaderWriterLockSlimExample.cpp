
#include <iostream>
#include <thread>
#include <vector>
#include <random>
#include <chrono>
#include "../include/System/Threading/ReaderWriterLockSlim.hpp"

using namespace System::Threading;

class SharedResource {
private:
    mutable ReaderWriterLockSlim lock_;
    int value_;
    
public:
    SharedResource() : value_(0) {}
    
    int read() const {
        lock_.enter_read_lock();
        try {
            std::this_thread::sleep_for(std::chrono::milliseconds(10)); // Simulate read work
            int result = value_;
            lock_.exit_read_lock();
            return result;
        } catch (...) {
            lock_.exit_read_lock();
            throw;
        }
    }
    
    void write(int new_value) {
        lock_.enter_write_lock();
        try {
            std::this_thread::sleep_for(std::chrono::milliseconds(50)); // Simulate write work
            value_ = new_value;
            lock_.exit_write_lock();
        } catch (...) {
            lock_.exit_write_lock();
            throw;
        }
    }
    
    void increment() {
        lock_.enter_upgradeable_read_lock();
        try {
            // Read current value
            int current = value_;
            
            // Upgrade to write lock
            lock_.exit_upgradeable_read_lock();
            lock_.enter_write_lock();
            
            // Write new value
            value_ = current + 1;
            lock_.exit_write_lock();
        } catch (...) {
            if (lock_.is_upgradeable_read_lock_held()) {
                lock_.exit_upgradeable_read_lock();
            }
            if (lock_.is_write_lock_held()) {
                lock_.exit_write_lock();
            }
            throw;
        }
    }
};

void reader_function(int id, SharedResource& resource, int iterations) {
    for (int i = 0; i < iterations; ++i) {
        int value = resource.read();
        std::cout << "Reader " << id << " read value: " << value << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }
}

void writer_function(int id, SharedResource& resource, int iterations) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 100);
    
    for (int i = 0; i < iterations; ++i) {
        int new_value = dis(gen);
        resource.write(new_value);
        std::cout << "Writer " << id << " wrote value: " << new_value << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void incrementer_function(int id, SharedResource& resource, int iterations) {
    for (int i = 0; i < iterations; ++i) {
        resource.increment();
        std::cout << "Incrementer " << id << " incremented value" << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(80));
    }
}

int main() {
    std::cout << "ReaderWriterLockSlim Example" << std::endl;
    std::cout << "============================" << std::endl;
    
    SharedResource resource;
    std::vector<std::thread> threads;
    
    // Create reader threads
    for (int i = 1; i <= 3; ++i) {
        threads.emplace_back(reader_function, i, std::ref(resource), 5);
    }
    
    // Create writer threads
    for (int i = 1; i <= 2; ++i) {
        threads.emplace_back(writer_function, i, std::ref(resource), 3);
    }
    
    // Create incrementer threads
    for (int i = 1; i <= 2; ++i) {
        threads.emplace_back(incrementer_function, i, std::ref(resource), 3);
    }
    
    // Wait for all threads to complete
    for (auto& thread : threads) {
        thread.join();
    }
    
    std::cout << "Final value: " << resource.read() << std::endl;
    
    return 0;
}
