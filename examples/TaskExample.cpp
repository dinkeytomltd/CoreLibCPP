
#include <iostream>
#include <vector>
#include "../include/System/Threading/Task.hpp"

using namespace System::Threading;

int compute_fibonacci(int n) {
    if (n <= 1) return n;
    return compute_fibonacci(n - 1) + compute_fibonacci(n - 2);
}

int main() {
    std::cout << "Task Example" << std::endl;
    std::cout << "============" << std::endl;
    
    // Example 1: Simple task
    std::cout << "\n1. Simple Task:" << std::endl;
    auto simple_task = Task::run([]() {
        std::cout << "Hello from task!" << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        std::cout << "Task completed!" << std::endl;
    });
    
    simple_task.wait();
    std::cout << "Simple task status: " << (int)simple_task.get_status() << std::endl;
    
    // Example 2: Task with return value
    std::cout << "\n2. Task with Return Value:" << std::endl;
    auto fibonacci_task = Task::run<int>([]() {
        return compute_fibonacci(10);
    });
    
    int result = fibonacci_task.get_result();
    std::cout << "Fibonacci(10) = " << result << std::endl;
    
    // Example 3: Task continuation
    std::cout << "\n3. Task Continuation:" << std::endl;
    auto continuation_task = fibonacci_task.continue_with<std::string>([](Task<int>& antecedent) {
        int fib_result = antecedent.get_result();
        return "The result was: " + std::to_string(fib_result);
    });
    
    std::string continuation_result = continuation_task.get_result();
    std::cout << continuation_result << std::endl;
    
    // Example 4: Multiple tasks
    std::cout << "\n4. Multiple Tasks:" << std::endl;
    std::vector<Task<int>> tasks;
    
    for (int i = 5; i <= 8; ++i) {
        tasks.push_back(Task::run<int>([i]() {
            std::cout << "Computing Fibonacci(" << i << ")..." << std::endl;
            return compute_fibonacci(i);
        }));
    }
    
    // Wait for all tasks and collect results
    std::cout << "Results:" << std::endl;
    for (size_t i = 0; i < tasks.size(); ++i) {
        int result = tasks[i].get_result();
        std::cout << "Fibonacci(" << (i + 5) << ") = " << result << std::endl;
    }
    
    // Example 5: Task delay
    std::cout << "\n5. Task Delay:" << std::endl;
    auto start_time = std::chrono::steady_clock::now();
    
    auto delay_task = Task::delay(500);
    delay_task.wait();
    
    auto end_time = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
    std::cout << "Delay completed after " << duration.count() << "ms" << std::endl;
    
    // Example 6: Exception handling
    std::cout << "\n6. Exception Handling:" << std::endl;
    auto exception_task = Task::run([]() {
        throw std::runtime_error("Something went wrong!");
    });
    
    try {
        exception_task.wait();
    } catch (const std::exception& e) {
        std::cout << "Caught exception: " << e.what() << std::endl;
    }
    
    std::cout << "Exception task status: " << (int)exception_task.get_status() << std::endl;
    
    return 0;
}
