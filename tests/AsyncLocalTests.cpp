
#include <iostream>
#include <thread>
#include <vector>
#include <cassert>
#include "../include/System/Threading/AsyncLocal.hpp"

using namespace System::Threading;

void test_basic_functionality() {
    AsyncLocal<int> async_local;
    
    // Initially should not have a value
    assert(!async_local.has_value());
    
    // Set a value
    async_local.set(42);
    assert(async_local.has_value());
    assert(async_local.get() == 42);
    
    // Clear the value
    async_local.clear();
    assert(!async_local.has_value());
    
    std::cout << "Basic functionality test passed" << std::endl;
}

void test_thread_isolation() {
    AsyncLocal<int> async_local;
    std::vector<std::thread> threads;
    std::vector<bool> results(5, false);
    
    for (int i = 0; i < 5; ++i) {
        threads.emplace_back([&async_local, &results, i]() {
            // Each thread sets its own value
            async_local.set(i * 10);
            
            // Verify the value is correct for this thread
            if (async_local.get() == i * 10) {
                results[i] = true;
            }
        });
    }
    
    for (auto& thread : threads) {
        thread.join();
    }
    
    // Verify all threads had their own values
    for (bool result : results) {
        assert(result);
    }
    
    std::cout << "Thread isolation test passed" << std::endl;
}

void test_value_factory() {
    AsyncLocal<std::string> async_local([]() {
        return std::string("default_value");
    });
    
    // Should get the factory value on first access
    assert(async_local.get() == "default_value");
    
    // Set a different value
    async_local.set("custom_value");
    assert(async_local.get() == "custom_value");
    
    std::cout << "Value factory test passed" << std::endl;
}

int main() {
    std::cout << "Running AsyncLocal tests..." << std::endl;
    
    test_basic_functionality();
    test_thread_isolation();
    test_value_factory();
    
    std::cout << "All AsyncLocal tests passed!" << std::endl;
    return 0;
}
