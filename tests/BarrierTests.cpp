
#include <iostream>
#include <thread>
#include <vector>
#include <cassert>
#include <chrono>
#include "../include/System/Threading/Barrier.hpp"

using namespace System::Threading;

void test_basic_functionality() {
    const int num_participants = 3;
    Barrier barrier(num_participants);
    std::vector<std::thread> threads;
    std::vector<bool> phase1_completed(num_participants, false);
    std::vector<bool> phase2_completed(num_participants, false);
    
    for (int i = 0; i < num_participants; ++i) {
        threads.emplace_back([&barrier, &phase1_completed, &phase2_completed, i]() {
            // Phase 1
            std::this_thread::sleep_for(std::chrono::milliseconds(i * 10));
            phase1_completed[i] = true;
            barrier.signal_and_wait();
            
            // Phase 2
            std::this_thread::sleep_for(std::chrono::milliseconds(i * 10));
            phase2_completed[i] = true;
            barrier.signal_and_wait();
        });
    }
    
    for (auto& thread : threads) {
        thread.join();
    }
    
    // All phases should be completed
    for (bool completed : phase1_completed) {
        assert(completed);
    }
    for (bool completed : phase2_completed) {
        assert(completed);
    }
    
    assert(barrier.get_current_phase_number() == 2);
    
    std::cout << "Basic functionality test passed" << std::endl;
}

void test_post_phase_action() {
    const int num_participants = 2;
    int post_phase_count = 0;
    
    Barrier barrier(num_participants, [&post_phase_count](Barrier&) {
        ++post_phase_count;
    });
    
    std::vector<std::thread> threads;
    
    for (int i = 0; i < num_participants; ++i) {
        threads.emplace_back([&barrier]() {
            barrier.signal_and_wait(); // Phase 0 -> 1
            barrier.signal_and_wait(); // Phase 1 -> 2
        });
    }
    
    for (auto& thread : threads) {
        thread.join();
    }
    
    assert(post_phase_count == 2);
    
    std::cout << "Post-phase action test passed" << std::endl;
}

void test_add_remove_participants() {
    Barrier barrier(1);
    
    // Add a participant
    long count = barrier.add_participant();
    assert(count == 2);
    assert(barrier.get_participant_count() == 2);
    
    // Remove a participant
    barrier.remove_participant();
    assert(barrier.get_participant_count() == 1);
    
    std::cout << "Add/remove participants test passed" << std::endl;
}

void test_timeout() {
    Barrier barrier(2); // Need 2 participants but only 1 will signal
    
    auto start_time = std::chrono::steady_clock::now();
    bool result = barrier.signal_and_wait(100); // 100ms timeout
    auto end_time = std::chrono::steady_clock::now();
    
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
    
    assert(!result); // Should timeout
    assert(duration.count() >= 100); // Should take at least 100ms
    
    std::cout << "Timeout test passed" << std::endl;
}

int main() {
    std::cout << "Running Barrier tests..." << std::endl;
    
    test_basic_functionality();
    test_post_phase_action();
    test_add_remove_participants();
    test_timeout();
    
    std::cout << "All Barrier tests passed!" << std::endl;
    return 0;
}
