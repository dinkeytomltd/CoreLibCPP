
#include <iostream>
#include <thread>
#include <vector>
#include <random>
#include "../include/System/Threading/Barrier.hpp"

using namespace System::Threading;

void worker_function(int id, Barrier& barrier) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(100, 500);
    
    for (int phase = 0; phase < 3; ++phase) {
        // Simulate work
        int work_time = dis(gen);
        std::cout << "Worker " << id << " working for " << work_time << "ms in phase " << phase << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(work_time));
        
        std::cout << "Worker " << id << " finished phase " << phase << ", waiting at barrier" << std::endl;
        
        // Wait for all workers to reach this point
        barrier.signal_and_wait();
        
        std::cout << "Worker " << id << " proceeding from phase " << phase << std::endl;
    }
}

int main() {
    std::cout << "Barrier Example" << std::endl;
    std::cout << "===============" << std::endl;
    
    const int num_workers = 4;
    
    // Create barrier with post-phase action
    Barrier barrier(num_workers, [](Barrier& b) {
        std::cout << "*** All workers completed phase " << b.get_current_phase_number() - 1 << " ***" << std::endl;
    });
    
    // Create worker threads
    std::vector<std::thread> threads;
    
    for (int i = 1; i <= num_workers; ++i) {
        threads.emplace_back(worker_function, i, std::ref(barrier));
    }
    
    // Wait for all threads to complete
    for (auto& thread : threads) {
        thread.join();
    }
    
    std::cout << "All workers completed all phases!" << std::endl;
    
    return 0;
}
