
#include <iostream>
#include <thread>
#include <vector>
#include "../include/System/Threading/ThreadLocal.hpp"

using namespace System::Threading;

ThreadLocal<std::string> threadName([]() { return std::string("Unnamed"); });
ThreadLocal<int> threadCounter;

void worker_function(int id) {
    // Set thread-specific values
    threadName.set("Worker-" + std::to_string(id));
    
    for (int i = 0; i < 5; ++i) {
        threadCounter.set(threadCounter.get() + 1);
        std::cout << threadName.get() << " counter: " << threadCounter.get() << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}

int main() {
    std::cout << "ThreadLocal Example" << std::endl;
    std::cout << "===================" << std::endl;
    
    // Set main thread name
    threadName.set("MainThread");
    std::cout << "Main thread name: " << threadName.get() << std::endl;
    
    // Create worker threads
    std::vector<std::thread> threads;
    
    for (int i = 1; i <= 3; ++i) {
        threads.emplace_back(worker_function, i);
    }
    
    // Wait for all threads to complete
    for (auto& thread : threads) {
        thread.join();
    }
    
    std::cout << "Main thread name after workers: " << threadName.get() << std::endl;
    std::cout << "Main thread counter: " << threadCounter.get() << std::endl;
    
    return 0;
}
