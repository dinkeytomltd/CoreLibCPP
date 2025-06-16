
#include <iostream>
#include <thread>
#include <vector>
#include <cassert>
#include <chrono>
#include "../include/System/Threading/CountdownEvent.hpp"

using namespace System::Threading;

void test_basic_functionality() {
    CountdownEvent countdown(3);
    
    assert(countdown.get_current_count() == 3);
    assert(countdown.get_initial_count() == 3);
    assert(!countdown.is_set());
    
    // Signal once
    bool result = countdown.signal();
    assert(result);
    assert(countdown.get_current_count() == 2);
    assert(!countdown.is_set());
    
    // Signal twice more
    countdown.signal();
    countdown.signal();
    
    assert(countdown.get_current_count() == 0);
    assert(countdown.is_set());
    
    // Additional signals should return false
    result = countdown.signal();
    assert(!result);
    
    std::cout << "Basic functionality test passed" << std::endl;
}

void test_multiple_signals() {
    CountdownEvent countdown(5);
    
    // Signal multiple at once
    bool result = countdown.signal(3);
    assert(result);
    assert(countdown.get_current_count() == 2);
    
    // Signal remaining
    countdown.signal(2);
    assert(countdown.is_set());
    
    std::cout << "Multiple signals test passed" << std::endl;
}

void test_wait_functionality() {
    CountdownEvent countdown(2);
    bool wait_completed = false;
    
    // Start a thread that will wait
    std::thread waiter([&countdown, &wait_completed]() {
        countdown.wait();
        wait_completed = true;
    });
    
    // Give the waiter time to start waiting
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    assert(!wait_completed);
    
    // Signal to complete the countdown
    countdown.signal(2);
    
    waiter.join();
    assert(wait_completed);
    
    std::cout << "Wait functionality test passed" << std::endl;
}

void test_timeout() {
    CountdownEvent countdown(1);
    
    auto start_time = std::chrono::steady_clock::now();
    bool result = countdown.wait(100); // 100ms timeout
    auto end_time = std::chrono::steady_clock::now();
    
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
    
    assert(!result); // Should timeout
    assert(duration.count() >= 100); // Should take at least 100ms
    
    std::cout << "Timeout test passed" << std::endl;
}

void test_add_count() {
    CountdownEvent countdown(1);
    
    // Add count
    countdown.add_count(2);
    assert(countdown.get_current_count() == 3);
    
    // Try to add count when set (should throw)
    countdown.signal(3);
    assert(countdown.is_set());
    
    try {
        countdown.add_count();
        assert(false); // Should not reach here
    } catch (const std::runtime_error&) {
        // Expected
    }
    
    std::cout << "Add count test passed" << std::endl;
}

void test_try_add_count() {
    CountdownEvent countdown(1);
    
    // Try add count (should succeed)
    bool result = countdown.try_add_count(2);
    assert(result);
    assert(countdown.get_current_count() == 3);
    
    // Set the countdown
    countdown.signal(3);
    assert(countdown.is_set());
    
    // Try add count when set (should fail)
    result = countdown.try_add_count();
    assert(!result);
    
    std::cout << "Try add count test passed" << std::endl;
}

void test_reset() {
    CountdownEvent countdown(3);
    
    // Signal and then reset
    countdown.signal(2);
    assert(countdown.get_current_count() == 1);
    
    countdown.reset();
    assert(countdown.get_current_count() == 3);
    assert(countdown.get_initial_count() == 3);
    
    // Reset with different count
    countdown.reset(5);
    assert(countdown.get_current_count() == 5);
    assert(countdown.get_initial_count() == 5);
    
    std::cout << "Reset test passed" << std::endl;
}

int main() {
    std::cout << "Running CountdownEvent tests..." << std::endl;
    
    test_basic_functionality();
    test_multiple_signals();
    test_wait_functionality();
    test_timeout();
    test_add_count();
    test_try_add_count();
    test_reset();
    
    std::cout << "All CountdownEvent tests passed!" << std::endl;
    return 0;
}
