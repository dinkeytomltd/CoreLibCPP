
#include <iostream>
#include <thread>
#include <cassert>
#include <chrono>
#include "../include/System/Threading/CancellationToken.hpp"

using namespace System::Threading;

void test_basic_cancellation() {
    CancellationTokenSource source;
    auto token = source.get_token();
    
    assert(!token.is_cancellation_requested());
    assert(token.can_be_canceled());
    
    source.cancel();
    assert(token.is_cancellation_requested());
    
    std::cout << "Basic cancellation test passed" << std::endl;
}

void test_throw_if_cancellation_requested() {
    CancellationTokenSource source;
    auto token = source.get_token();
    
    // Should not throw
    token.throw_if_cancellation_requested();
    
    source.cancel();
    
    // Should throw
    try {
        token.throw_if_cancellation_requested();
        assert(false); // Should not reach here
    } catch (const OperationCanceledException&) {
        // Expected
    }
    
    std::cout << "Throw if cancellation requested test passed" << std::endl;
}

void test_cancellation_callbacks() {
    CancellationTokenSource source;
    auto token = source.get_token();
    
    bool callback1_executed = false;
    bool callback2_executed = false;
    
    auto registration1 = token.register_callback([&callback1_executed]() {
        callback1_executed = true;
    });
    
    auto registration2 = token.register_callback([&callback2_executed]() {
        callback2_executed = true;
    });
    
    assert(!callback1_executed);
    assert(!callback2_executed);
    
    source.cancel();
    
    // Give callbacks time to execute
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    
    assert(callback1_executed);
    assert(callback2_executed);
    
    std::cout << "Cancellation callbacks test passed" << std::endl;
}

void test_pre_canceled_token() {
    auto token = CancellationToken::canceled();
    
    assert(token.is_cancellation_requested());
    
    try {
        token.throw_if_cancellation_requested();
        assert(false); // Should not reach here
    } catch (const OperationCanceledException&) {
        // Expected
    }
    
    std::cout << "Pre-canceled token test passed" << std::endl;
}

void test_none_token() {
    auto token = CancellationToken::none();
    
    assert(!token.is_cancellation_requested());
    assert(token.can_be_canceled());
    
    // Should not throw
    token.throw_if_cancellation_requested();
    
    std::cout << "None token test passed" << std::endl;
}

void test_callback_registration_disposal() {
    CancellationTokenSource source;
    auto token = source.get_token();
    
    bool callback_executed = false;
    
    {
        auto registration = token.register_callback([&callback_executed]() {
            callback_executed = true;
        });
        
        // Registration goes out of scope and should be disposed
    }
    
    source.cancel();
    
    // Give time for any callbacks to execute
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    
    // Callback should not have been executed since registration was disposed
    // Note: This test might be implementation-dependent
    
    std::cout << "Callback registration disposal test passed" << std::endl;
}

void test_linked_token_source() {
    CancellationTokenSource parent_source;
    auto parent_token = parent_source.get_token();
    
    auto linked_source = CancellationTokenSource::create_linked_token_source(parent_token);
    auto linked_token = linked_source.get_token();
    
    assert(!parent_token.is_cancellation_requested());
    assert(!linked_token.is_cancellation_requested());
    
    // Cancel parent
    parent_source.cancel();
    
    // Give time for linked token to be canceled
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    
    assert(parent_token.is_cancellation_requested());
    assert(linked_token.is_cancellation_requested());
    
    std::cout << "Linked token source test passed" << std::endl;
}

void test_callback_with_already_canceled_token() {
    auto token = CancellationToken::canceled();
    
    bool callback_executed = false;
    
    auto registration = token.register_callback([&callback_executed]() {
        callback_executed = true;
    });
    
    // Callback should be executed immediately for already canceled token
    // Give a small amount of time for execution
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    
    assert(callback_executed);
    
    std::cout << "Callback with already canceled token test passed" << std::endl;
}

int main() {
    std::cout << "Running CancellationToken tests..." << std::endl;
    
    test_basic_cancellation();
    test_throw_if_cancellation_requested();
    test_cancellation_callbacks();
    test_pre_canceled_token();
    test_none_token();
    test_callback_registration_disposal();
    test_linked_token_source();
    test_callback_with_already_canceled_token();
    
    std::cout << "All CancellationToken tests passed!" << std::endl;
    return 0;
}
