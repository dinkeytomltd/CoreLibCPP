
#include <iostream>
#include <thread>
#include <vector>
#include "../include/System/Threading/AsyncLocal.hpp"

using namespace System::Threading;

AsyncLocal<int> asyncValue;

void worker_function(int id) {
    // Set a value specific to this execution context
    asyncValue.set(id * 100);
    
    std::cout << "Thread " << id << " set value: " << asyncValue.get() << std::endl;
    
    // Simulate some work
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    
    std::cout << "Thread " << id << " still has value: " << asyncValue.get() << std::endl;
}

int main() {
    std::cout << "AsyncLocal Example" << std::endl;
    std::cout << "==================" << std::endl;
    
    // Create multiple threads
    std::vector<std::thread> threads;
    
    for (int i = 1; i <= 5; ++i) {
        threads.emplace_back(worker_function, i);
    }
    
    // Wait for all threads to complete
    for (auto& thread : threads) {
        thread.join();
    }
    
    // Main thread should not have a value set
    std::cout << "Main thread has value: " << asyncValue.has_value() << std::endl;
    
    return 0;
}
