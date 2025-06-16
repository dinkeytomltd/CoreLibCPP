
#include <iostream>
#include <thread>
#include <vector>
#include <random>
#include "../include/System/Threading/CountdownEvent.hpp"

using namespace System::Threading;

void worker_function(int id, CountdownEvent& countdown) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(100, 1000);
    
    // Simulate work
    int work_time = dis(gen);
    std::cout << "Worker " << id << " starting work (" << work_time << "ms)" << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(work_time));
    
    std::cout << "Worker " << id << " completed work, signaling countdown" << std::endl;
    countdown.signal();
}

int main() {
    std::cout << "CountdownEvent Example" << std::endl;
    std::cout << "======================" << std::endl;
    
    const int num_workers = 5;
    CountdownEvent countdown(num_workers);
    
    std::cout << "Starting " << num_workers << " workers..." << std::endl;
    std::cout << "Initial countdown: " << countdown.get_current_count() << std::endl;
    
    // Create worker threads
    std::vector<std::thread> threads;
    
    for (int i = 1; i <= num_workers; ++i) {
        threads.emplace_back(worker_function, i, std::ref(countdown));
    }
    
    // Monitor countdown progress
    std::thread monitor([&countdown]() {
        while (!countdown.is_set()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
            std::cout << "Remaining count: " << countdown.get_current_count() << std::endl;
        }
    });
    
    // Wait for all workers to complete
    std::cout << "Main thread waiting for all workers to complete..." << std::endl;
    countdown.wait();
    
    std::cout << "All workers completed!" << std::endl;
    
    // Clean up
    monitor.join();
    for (auto& thread : threads) {
        thread.join();
    }
    
    return 0;
}
