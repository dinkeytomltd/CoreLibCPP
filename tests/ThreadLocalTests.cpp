
#include <iostream>
#include <thread>
#include <vector>
#include <cassert>
#include "../include/System/Threading/ThreadLocal.hpp"

using namespace System::Threading;

void test_basic_functionality() {
    ThreadLocal<int> thread_local;
    
    // Initially should not be created
    assert(!thread_local.is_value_created());
    
    // Access should create default value
    int& value = thread_local.get();
    assert(thread_local.is_value_created());
    assert(value == 0); // Default int value
    
    // Set a value
    thread_local.set(42);
    assert(thread_local.get() == 42);
    
    std::cout << "Basic functionality test passed" << std::endl;
}

void test_thread_isolation() {
    ThreadLocal<int> thread_local;
    std::vector<std::thread> threads;
    std::vector<bool> results(5, false);
    
    for (int i = 0; i < 5; ++i) {
        threads.emplace_back([&thread_local, &results, i]() {
            // Each thread sets its own value
            thread_local.set(i * 10);
            
            // Verify the value is correct for this thread
            if (thread_local.get() == i * 10) {
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
    ThreadLocal<std::string> thread_local([]() {
        return std::string("factory_value");
    });
    
    // Should get the factory value on first access
    assert(thread_local.get() == "factory_value");
    
    // Set a different value
    thread_local.set("custom_value");
    assert(thread_local.get() == "custom_value");
    
    std::cout << "Value factory test passed" << std::endl;
}

void test_operators() {
    ThreadLocal<int> thread_local;
    
    // Test dereference operator
    *thread_local = 100;
    assert(*thread_local == 100);
    
    // Test arrow operator (with a struct)
    struct TestStruct {
        int value = 0;
        void set_value(int v) { value = v; }
    };
    
    ThreadLocal<TestStruct> struct_local;
    struct_local->set_value(200);
    assert(struct_local->value == 200);
    
    std::cout << "Operators test passed" << std::endl;
}

int main() {
    std::cout << "Running ThreadLocal tests..." << std::endl;
    
    test_basic_functionality();
    test_thread_isolation();
    test_value_factory();
    test_operators();
    
    std::cout << "All ThreadLocal tests passed!" << std::endl;
    return 0;
}
