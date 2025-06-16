
#include <iostream>
#include <cassert>
#include <chrono>
#include "../include/System/Threading/Task.hpp"

using namespace System::Threading;

void test_basic_task() {
    auto task = Task::run([]() {
        return 42;
    });
    
    int result = task.get_result();
    assert(result == 42);
    assert(task.is_completed_successfully());
    assert(task.get_status() == TaskStatus::RanToCompletion);
    
    std::cout << "Basic task test passed" << std::endl;
}

void test_void_task() {
    bool executed = false;
    
    auto task = Task::run([&executed]() {
        executed = true;
    });
    
    task.wait();
    assert(executed);
    assert(task.is_completed_successfully());
    
    std::cout << "Void task test passed" << std::endl;
}

void test_task_exception() {
    auto task = Task::run([]() {
        throw std::runtime_error("Test exception");
        return 42;
    });
    
    try {
        task.get_result();
        assert(false); // Should not reach here
    } catch (const std::runtime_error& e) {
        assert(std::string(e.what()) == "Test exception");
    }
    
    assert(task.is_faulted());
    assert(task.get_status() == TaskStatus::Faulted);
    
    std::cout << "Task exception test passed" << std::endl;
}

void test_task_continuation() {
    auto task1 = Task::run([]() {
        return 10;
    });
    
    auto task2 = task1.continue_with<int>([](Task<int>& antecedent) {
        int result = antecedent.get_result();
        return result * 2;
    });
    
    int final_result = task2.get_result();
    assert(final_result == 20);
    
    std::cout << "Task continuation test passed" << std::endl;
}

void test_task_delay() {
    auto start_time = std::chrono::steady_clock::now();
    
    auto delay_task = Task::delay(100);
    delay_task.wait();
    
    auto end_time = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
    
    assert(duration.count() >= 100);
    assert(delay_task.is_completed_successfully());
    
    std::cout << "Task delay test passed" << std::endl;
}

void test_completed_task() {
    auto task = Task::completed_task();
    
    assert(task.is_completed());
    assert(task.is_completed_successfully());
    
    std::cout << "Completed task test passed" << std::endl;
}

void test_from_result() {
    auto task = Task::from_result(100);
    
    int result = task.get_result();
    assert(result == 100);
    assert(task.is_completed_successfully());
    
    std::cout << "From result test passed" << std::endl;
}

void test_from_exception() {
    auto exception = std::make_exception_ptr(std::runtime_error("Test error"));
    auto task = Task::from_exception(exception);
    
    try {
        task.wait();
        assert(false); // Should not reach here
    } catch (const std::runtime_error& e) {
        assert(std::string(e.what()) == "Test error");
    }
    
    assert(task.is_faulted());
    
    std::cout << "From exception test passed" << std::endl;
}

void test_wait_timeout() {
    auto task = Task::delay(200);
    
    // Should timeout
    bool result = task.wait(50);
    assert(!result);
    
    // Should complete
    result = task.wait(200);
    assert(result);
    
    std::cout << "Wait timeout test passed" << std::endl;
}

int main() {
    std::cout << "Running Task tests..." << std::endl;
    
    test_basic_task();
    test_void_task();
    test_task_exception();
    test_task_continuation();
    test_task_delay();
    test_completed_task();
    test_from_result();
    test_from_exception();
    test_wait_timeout();
    
    std::cout << "All Task tests passed!" << std::endl;
    return 0;
}
