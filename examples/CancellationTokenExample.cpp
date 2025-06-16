
#include <iostream>
#include <thread>
#include <chrono>
#include "../include/System/Threading/CancellationToken.hpp"
#include "../include/System/Threading/Task.hpp"

using namespace System::Threading;

void cancellable_operation(CancellationToken token, int id) {
    try {
        for (int i = 0; i < 10; ++i) {
            // Check for cancellation
            token.throw_if_cancellation_requested();
            
            std::cout << "Operation " << id << " - Step " << i << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
        }
        
        std::cout << "Operation " << id << " completed successfully!" << std::endl;
    } catch (const OperationCanceledException& e) {
        std::cout << "Operation " << id << " was canceled: " << e.what() << std::endl;
    }
}

int main() {
    std::cout << "CancellationToken Example" << std::endl;
    std::cout << "=========================" << std::endl;
    
    // Example 1: Basic cancellation
    std::cout << "\n1. Basic Cancellation:" << std::endl;
    {
        CancellationTokenSource source;
        auto token = source.get_token();
        
        // Start a cancellable operation
        std::thread worker(cancellable_operation, token, 1);
        
        // Cancel after 1 second
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        std::cout << "Requesting cancellation..." << std::endl;
        source.cancel();
        
        worker.join();
    }
    
    // Example 2: Cancellation callbacks
    std::cout << "\n2. Cancellation Callbacks:" << std::endl;
    {
        CancellationTokenSource source;
        auto token = source.get_token();
        
        // Register cancellation callbacks
        auto registration1 = token.register_callback([]() {
            std::cout << "Callback 1: Cleanup resources" << std::endl;
        });
        
        auto registration2 = token.register_callback([]() {
            std::cout << "Callback 2: Log cancellation" << std::endl;
        });
        
        std::cout << "Canceling token..." << std::endl;
        source.cancel();
        
        std::cout << "Token is canceled: " << token.is_cancellation_requested() << std::endl;
    }
    
    // Example 3: Task with cancellation
    std::cout << "\n3. Task with Cancellation:" << std::endl;
    {
        CancellationTokenSource source;
        auto token = source.get_token();
        
        auto task = Task::run([token]() {
            for (int i = 0; i < 20; ++i) {
                token.throw_if_cancellation_requested();
                std::cout << "Task working... " << i << std::endl;
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
            return 42;
        }, token);
        
        // Cancel after 500ms
        std::thread canceller([&source]() {
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            std::cout << "Canceling task..." << std::endl;
            source.cancel();
        });
        
        try {
            task.wait();
            std::cout << "Task completed normally" << std::endl;
        } catch (const OperationCanceledException& e) {
            std::cout << "Task was canceled: " << e.what() << std::endl;
        }
        
        canceller.join();
        std::cout << "Task status: " << (int)task.get_status() << std::endl;
    }
    
    // Example 4: Linked cancellation tokens
    std::cout << "\n4. Linked Cancellation Tokens:" << std::endl;
    {
        CancellationTokenSource parent_source;
        auto parent_token = parent_source.get_token();
        
        auto linked_source = CancellationTokenSource::create_linked_token_source(parent_token);
        auto linked_token = linked_source.get_token();
        
        // Register callback on linked token
        auto registration = linked_token.register_callback([]() {
            std::cout << "Linked token was canceled!" << std::endl;
        });
        
        std::cout << "Canceling parent token..." << std::endl;
        parent_source.cancel();
        
        std::cout << "Parent token canceled: " << parent_token.is_cancellation_requested() << std::endl;
        std::cout << "Linked token canceled: " << linked_token.is_cancellation_requested() << std::endl;
    }
    
    // Example 5: Pre-canceled token
    std::cout << "\n5. Pre-canceled Token:" << std::endl;
    {
        auto canceled_token = CancellationToken::canceled();
        std::cout << "Pre-canceled token is canceled: " << canceled_token.is_cancellation_requested() << std::endl;
        
        try {
            canceled_token.throw_if_cancellation_requested();
        } catch (const OperationCanceledException& e) {
            std::cout << "Pre-canceled token threw: " << e.what() << std::endl;
        }
    }
    
    return 0;
}
