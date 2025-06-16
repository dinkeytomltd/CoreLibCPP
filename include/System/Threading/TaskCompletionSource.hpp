
#pragma once

#include "Task.hpp"
#include <promise>
#include <future>
#include <memory>

namespace System {
namespace Threading {

template<typename T>
class TaskCompletionSource {
private:
    std::shared_ptr<std::promise<T>> promise_;
    Task<T> task_;
    std::atomic<bool> completed_;
    
public:
    TaskCompletionSource() 
        : promise_(std::make_shared<std::promise<T>>())
        , task_(promise_->get_future().share())
        , completed_(false) {}
    
    explicit TaskCompletionSource(TaskCreationOptions options)
        : promise_(std::make_shared<std::promise<T>>())
        , task_(promise_->get_future().share(), CancellationToken::none(), options)
        , completed_(false) {}
    
    TaskCompletionSource(void* state, TaskCreationOptions options)
        : promise_(std::make_shared<std::promise<T>>())
        , task_(promise_->get_future().share(), CancellationToken::none(), options)
        , completed_(false) {
        // Note: state parameter is for compatibility but not used in this implementation
        (void)state;
    }
    
    // Disable copy constructor and assignment
    TaskCompletionSource(const TaskCompletionSource&) = delete;
    TaskCompletionSource& operator=(const TaskCompletionSource&) = delete;
    
    // Move constructor and assignment
    TaskCompletionSource(TaskCompletionSource&& other) noexcept
        : promise_(std::move(other.promise_))
        , task_(std::move(other.task_))
        , completed_(other.completed_.load()) {}
    
    TaskCompletionSource& operator=(TaskCompletionSource&& other) noexcept {
        if (this != &other) {
            promise_ = std::move(other.promise_);
            task_ = std::move(other.task_);
            completed_.store(other.completed_.load());
        }
        return *this;
    }
    
    // Get the associated task
    Task<T> get_task() const {
        return task_;
    }
    
    // Set the result
    void set_result(const T& result) {
        if (completed_.exchange(true)) {
            throw std::runtime_error("TaskCompletionSource has already been completed");
        }
        
        try {
            promise_->set_value(result);
        } catch (...) {
            completed_.store(false);
            throw;
        }
    }
    
    void set_result(T&& result) {
        if (completed_.exchange(true)) {
            throw std::runtime_error("TaskCompletionSource has already been completed");
        }
        
        try {
            promise_->set_value(std::move(result));
        } catch (...) {
            completed_.store(false);
            throw;
        }
    }
    
    // Try to set the result (returns false if already completed)
    bool try_set_result(const T& result) {
        if (completed_.exchange(true)) {
            return false;
        }
        
        try {
            promise_->set_value(result);
            return true;
        } catch (...) {
            completed_.store(false);
            return false;
        }
    }
    
    bool try_set_result(T&& result) {
        if (completed_.exchange(true)) {
            return false;
        }
        
        try {
            promise_->set_value(std::move(result));
            return true;
        } catch (...) {
            completed_.store(false);
            return false;
        }
    }
    
    // Set exception
    void set_exception(std::exception_ptr exception) {
        if (completed_.exchange(true)) {
            throw std::runtime_error("TaskCompletionSource has already been completed");
        }
        
        try {
            promise_->set_exception(exception);
        } catch (...) {
            completed_.store(false);
            throw;
        }
    }
    
    template<typename Exception>
    void set_exception(const Exception& exception) {
        set_exception(std::make_exception_ptr(exception));
    }
    
    // Try to set exception (returns false if already completed)
    bool try_set_exception(std::exception_ptr exception) {
        if (completed_.exchange(true)) {
            return false;
        }
        
        try {
            promise_->set_exception(exception);
            return true;
        } catch (...) {
            completed_.store(false);
            return false;
        }
    }
    
    template<typename Exception>
    bool try_set_exception(const Exception& exception) {
        return try_set_exception(std::make_exception_ptr(exception));
    }
    
    // Set canceled
    void set_canceled() {
        set_exception(std::make_exception_ptr(OperationCanceledException()));
    }
    
    void set_canceled(CancellationToken cancellationToken) {
        if (cancellationToken.is_cancellation_requested()) {
            set_exception(std::make_exception_ptr(OperationCanceledException()));
        } else {
            throw std::invalid_argument("CancellationToken is not in canceled state");
        }
    }
    
    // Try to set canceled (returns false if already completed)
    bool try_set_canceled() {
        return try_set_exception(std::make_exception_ptr(OperationCanceledException()));
    }
    
    bool try_set_canceled(CancellationToken cancellationToken) {
        if (cancellationToken.is_cancellation_requested()) {
            return try_set_exception(std::make_exception_ptr(OperationCanceledException()));
        }
        return false;
    }
    
    // Check if the task has been completed
    bool is_completed() const {
        return completed_.load();
    }
};

// Specialization for void
template<>
class TaskCompletionSource<void> {
private:
    std::shared_ptr<std::promise<void>> promise_;
    Task task_;
    std::atomic<bool> completed_;
    
public:
    TaskCompletionSource() 
        : promise_(std::make_shared<std::promise<void>>())
        , task_(promise_->get_future().share())
        , completed_(false) {}
    
    explicit TaskCompletionSource(TaskCreationOptions options)
        : promise_(std::make_shared<std::promise<void>>())
        , task_(promise_->get_future().share(), CancellationToken::none(), options)
        , completed_(false) {}
    
    TaskCompletionSource(void* state, TaskCreationOptions options)
        : promise_(std::make_shared<std::promise<void>>())
        , task_(promise_->get_future().share(), CancellationToken::none(), options)
        , completed_(false) {
        (void)state;
    }
    
    // Disable copy constructor and assignment
    TaskCompletionSource(const TaskCompletionSource&) = delete;
    TaskCompletionSource& operator=(const TaskCompletionSource&) = delete;
    
    // Move constructor and assignment
    TaskCompletionSource(TaskCompletionSource&& other) noexcept
        : promise_(std::move(other.promise_))
        , task_(std::move(other.task_))
        , completed_(other.completed_.load()) {}
    
    TaskCompletionSource& operator=(TaskCompletionSource&& other) noexcept {
        if (this != &other) {
            promise_ = std::move(other.promise_);
            task_ = std::move(other.task_);
            completed_.store(other.completed_.load());
        }
        return *this;
    }
    
    // Get the associated task
    Task get_task() const {
        return task_;
    }
    
    // Set the result (void)
    void set_result() {
        if (completed_.exchange(true)) {
            throw std::runtime_error("TaskCompletionSource has already been completed");
        }
        
        try {
            promise_->set_value();
        } catch (...) {
            completed_.store(false);
            throw;
        }
    }
    
    // Try to set the result (returns false if already completed)
    bool try_set_result() {
        if (completed_.exchange(true)) {
            return false;
        }
        
        try {
            promise_->set_value();
            return true;
        } catch (...) {
            completed_.store(false);
            return false;
        }
    }
    
    // Set exception
    void set_exception(std::exception_ptr exception) {
        if (completed_.exchange(true)) {
            throw std::runtime_error("TaskCompletionSource has already been completed");
        }
        
        try {
            promise_->set_exception(exception);
        } catch (...) {
            completed_.store(false);
            throw;
        }
    }
    
    template<typename Exception>
    void set_exception(const Exception& exception) {
        set_exception(std::make_exception_ptr(exception));
    }
    
    // Try to set exception (returns false if already completed)
    bool try_set_exception(std::exception_ptr exception) {
        if (completed_.exchange(true)) {
            return false;
        }
        
        try {
            promise_->set_exception(exception);
            return true;
        } catch (...) {
            completed_.store(false);
            return false;
        }
    }
    
    template<typename Exception>
    bool try_set_exception(const Exception& exception) {
        return try_set_exception(std::make_exception_ptr(exception));
    }
    
    // Set canceled
    void set_canceled() {
        set_exception(std::make_exception_ptr(OperationCanceledException()));
    }
    
    void set_canceled(CancellationToken cancellationToken) {
        if (cancellationToken.is_cancellation_requested()) {
            set_exception(std::make_exception_ptr(OperationCanceledException()));
        } else {
            throw std::invalid_argument("CancellationToken is not in canceled state");
        }
    }
    
    // Try to set canceled (returns false if already completed)
    bool try_set_canceled() {
        return try_set_exception(std::make_exception_ptr(OperationCanceledException()));
    }
    
    bool try_set_canceled(CancellationToken cancellationToken) {
        if (cancellationToken.is_cancellation_requested()) {
            return try_set_exception(std::make_exception_ptr(OperationCanceledException()));
        }
        return false;
    }
    
    // Check if the task has been completed
    bool is_completed() const {
        return completed_.load();
    }
};

} // namespace Threading
} // namespace System
