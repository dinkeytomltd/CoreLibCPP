
#pragma once

#include "Enums.hpp"
#include "Delegates.hpp"
#include "CancellationToken.hpp"
#include <future>
#include <memory>
#include <functional>
#include <exception>
#include <vector>
#include <thread>
#include <chrono>

namespace System {
namespace Threading {

// Forward declarations
template<typename T> class Task;
template<typename T> class TaskCompletionSource;

// Base Task class (non-generic)
class Task {
protected:
    std::shared_future<void> future_;
    TaskStatus status_;
    std::exception_ptr exception_;
    CancellationToken cancellation_token_;
    TaskCreationOptions creation_options_;
    mutable std::mutex status_mutex_;
    
    Task(std::shared_future<void> future, 
         CancellationToken token = CancellationToken::none(),
         TaskCreationOptions options = TaskCreationOptions::None)
        : future_(std::move(future))
        , status_(TaskStatus::Created)
        , cancellation_token_(token)
        , creation_options_(options) {}
    
public:
    Task() : status_(TaskStatus::Created) {}
    
    virtual ~Task() = default;
    
    // Copy and move constructors
    Task(const Task&) = default;
    Task& operator=(const Task&) = default;
    Task(Task&&) = default;
    Task& operator=(Task&&) = default;
    
    // Properties
    TaskStatus get_status() const {
        std::lock_guard<std::mutex> lock(status_mutex_);
        return status_;
    }
    
    bool is_completed() const {
        auto status = get_status();
        return status == TaskStatus::RanToCompletion || 
               status == TaskStatus::Faulted || 
               status == TaskStatus::Canceled;
    }
    
    bool is_completed_successfully() const {
        return get_status() == TaskStatus::RanToCompletion;
    }
    
    bool is_faulted() const {
        return get_status() == TaskStatus::Faulted;
    }
    
    bool is_canceled() const {
        return get_status() == TaskStatus::Canceled;
    }
    
    std::exception_ptr get_exception() const {
        std::lock_guard<std::mutex> lock(status_mutex_);
        return exception_;
    }
    
    CancellationToken get_cancellation_token() const {
        return cancellation_token_;
    }
    
    TaskCreationOptions get_creation_options() const {
        return creation_options_;
    }
    
    // Wait methods
    void wait() const {
        if (future_.valid()) {
            try {
                future_.wait();
                update_status_from_future();
            } catch (...) {
                std::lock_guard<std::mutex> lock(status_mutex_);
                status_ = TaskStatus::Faulted;
                exception_ = std::current_exception();
                throw;
            }
        }
    }
    
    bool wait(int millisecondsTimeout) const {
        if (!future_.valid()) {
            return true;
        }
        
        auto timeout = std::chrono::milliseconds(millisecondsTimeout);
        auto status = future_.wait_for(timeout);
        
        if (status == std::future_status::ready) {
            update_status_from_future();
            return true;
        }
        
        return false;
    }
    
    template<typename Rep, typename Period>
    bool wait(const std::chrono::duration<Rep, Period>& timeout) const {
        if (!future_.valid()) {
            return true;
        }
        
        auto status = future_.wait_for(timeout);
        
        if (status == std::future_status::ready) {
            update_status_from_future();
            return true;
        }
        
        return false;
    }
    
    // Continuation methods
    Task continue_with(std::function<void(Task&)> continuation,
                      TaskContinuationOptions options = TaskContinuationOptions::None) {
        return continue_with_impl(std::move(continuation), options);
    }
    
    template<typename TResult>
    Task<TResult> continue_with(std::function<TResult(Task&)> continuation,
                               TaskContinuationOptions options = TaskContinuationOptions::None) {
        return continue_with_impl(std::move(continuation), options);
    }
    
    // Static factory methods
    static Task run(std::function<void()> action, 
                   CancellationToken cancellationToken = CancellationToken::none(),
                   TaskCreationOptions options = TaskCreationOptions::None) {
        auto promise = std::make_shared<std::promise<void>>();
        auto future = promise->get_future().share();
        
        std::thread([promise, action = std::move(action), cancellationToken]() {
            try {
                cancellationToken.throw_if_cancellation_requested();
                action();
                promise->set_value();
            } catch (...) {
                promise->set_exception(std::current_exception());
            }
        }).detach();
        
        return Task(std::move(future), cancellationToken, options);
    }
    
    template<typename TResult>
    static Task<TResult> run(std::function<TResult()> function,
                            CancellationToken cancellationToken = CancellationToken::none(),
                            TaskCreationOptions options = TaskCreationOptions::None) {
        auto promise = std::make_shared<std::promise<TResult>>();
        auto future = promise->get_future().share();
        
        std::thread([promise, function = std::move(function), cancellationToken]() {
            try {
                cancellationToken.throw_if_cancellation_requested();
                if constexpr (std::is_void_v<TResult>) {
                    function();
                    promise->set_value();
                } else {
                    auto result = function();
                    promise->set_value(std::move(result));
                }
            } catch (...) {
                promise->set_exception(std::current_exception());
            }
        }).detach();
        
        return Task<TResult>(std::move(future), cancellationToken, options);
    }
    
    static Task completed_task() {
        auto promise = std::promise<void>();
        promise.set_value();
        return Task(promise.get_future().share());
    }
    
    static Task from_exception(std::exception_ptr exception) {
        auto promise = std::promise<void>();
        promise.set_exception(exception);
        return Task(promise.get_future().share());
    }
    
    template<typename TResult>
    static Task<TResult> from_result(TResult result) {
        auto promise = std::promise<TResult>();
        promise.set_value(std::move(result));
        return Task<TResult>(promise.get_future().share());
    }
    
    static Task from_canceled(CancellationToken cancellationToken) {
        auto promise = std::promise<void>();
        promise.set_exception(std::make_exception_ptr(OperationCanceledException()));
        return Task(promise.get_future().share(), cancellationToken);
    }
    
    // Wait for all tasks
    static void wait_all(const std::vector<Task>& tasks) {
        for (const auto& task : tasks) {
            task.wait();
        }
    }
    
    static bool wait_all(const std::vector<Task>& tasks, int millisecondsTimeout) {
        auto start_time = std::chrono::steady_clock::now();
        
        for (const auto& task : tasks) {
            auto elapsed = std::chrono::steady_clock::now() - start_time;
            auto remaining = std::chrono::milliseconds(millisecondsTimeout) - elapsed;
            
            if (remaining <= std::chrono::milliseconds(0) || !task.wait(remaining)) {
                return false;
            }
        }
        
        return true;
    }
    
    // Wait for any task
    static int wait_any(const std::vector<Task>& tasks) {
        // Simple implementation - in practice, this would be more sophisticated
        while (true) {
            for (size_t i = 0; i < tasks.size(); ++i) {
                if (tasks[i].is_completed()) {
                    return static_cast<int>(i);
                }
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    }
    
    static int wait_any(const std::vector<Task>& tasks, int millisecondsTimeout) {
        auto start_time = std::chrono::steady_clock::now();
        
        while (std::chrono::steady_clock::now() - start_time < std::chrono::milliseconds(millisecondsTimeout)) {
            for (size_t i = 0; i < tasks.size(); ++i) {
                if (tasks[i].is_completed()) {
                    return static_cast<int>(i);
                }
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
        
        return -1; // Timeout
    }
    
    // Delay
    static Task delay(int millisecondsDelay, CancellationToken cancellationToken = CancellationToken::none()) {
        return delay(std::chrono::milliseconds(millisecondsDelay), cancellationToken);
    }
    
    template<typename Rep, typename Period>
    static Task delay(const std::chrono::duration<Rep, Period>& delay, 
                     CancellationToken cancellationToken = CancellationToken::none()) {
        auto promise = std::make_shared<std::promise<void>>();
        auto future = promise->get_future().share();
        
        std::thread([promise, delay, cancellationToken]() {
            try {
                std::this_thread::sleep_for(delay);
                cancellationToken.throw_if_cancellation_requested();
                promise->set_value();
            } catch (...) {
                promise->set_exception(std::current_exception());
            }
        }).detach();
        
        return Task(std::move(future), cancellationToken);
    }
    
protected:
    void update_status_from_future() const {
        if (!future_.valid()) {
            return;
        }
        
        std::lock_guard<std::mutex> lock(status_mutex_);
        
        try {
            // This will throw if the future contains an exception
            future_.get();
            status_ = TaskStatus::RanToCompletion;
        } catch (const OperationCanceledException&) {
            status_ = TaskStatus::Canceled;
            exception_ = std::current_exception();
        } catch (...) {
            status_ = TaskStatus::Faulted;
            exception_ = std::current_exception();
        }
    }
    
private:
    Task continue_with_impl(std::function<void(Task&)> continuation,
                           TaskContinuationOptions options) {
        auto promise = std::make_shared<std::promise<void>>();
        auto future = promise->get_future().share();
        
        std::thread([this, promise, continuation = std::move(continuation), options]() {
            try {
                // Wait for this task to complete
                this->wait();
                
                // Check continuation options
                if (should_run_continuation(options)) {
                    continuation(*this);
                }
                
                promise->set_value();
            } catch (...) {
                promise->set_exception(std::current_exception());
            }
        }).detach();
        
        return Task(std::move(future), cancellation_token_);
    }
    
    template<typename TResult>
    Task<TResult> continue_with_impl(std::function<TResult(Task&)> continuation,
                                    TaskContinuationOptions options) {
        auto promise = std::make_shared<std::promise<TResult>>();
        auto future = promise->get_future().share();
        
        std::thread([this, promise, continuation = std::move(continuation), options]() {
            try {
                // Wait for this task to complete
                this->wait();
                
                // Check continuation options
                if (should_run_continuation(options)) {
                    if constexpr (std::is_void_v<TResult>) {
                        continuation(*this);
                        promise->set_value();
                    } else {
                        auto result = continuation(*this);
                        promise->set_value(std::move(result));
                    }
                } else {
                    if constexpr (std::is_void_v<TResult>) {
                        promise->set_value();
                    } else {
                        promise->set_value(TResult{});
                    }
                }
            } catch (...) {
                promise->set_exception(std::current_exception());
            }
        }).detach();
        
        return Task<TResult>(std::move(future), cancellation_token_);
    }
    
    bool should_run_continuation(TaskContinuationOptions options) const {
        auto status = get_status();
        
        if ((options & TaskContinuationOptions::NotOnRanToCompletion) != TaskContinuationOptions::None &&
            status == TaskStatus::RanToCompletion) {
            return false;
        }
        
        if ((options & TaskContinuationOptions::NotOnFaulted) != TaskContinuationOptions::None &&
            status == TaskStatus::Faulted) {
            return false;
        }
        
        if ((options & TaskContinuationOptions::NotOnCanceled) != TaskContinuationOptions::None &&
            status == TaskStatus::Canceled) {
            return false;
        }
        
        if ((options & TaskContinuationOptions::OnlyOnRanToCompletion) != TaskContinuationOptions::None &&
            status != TaskStatus::RanToCompletion) {
            return false;
        }
        
        if ((options & TaskContinuationOptions::OnlyOnFaulted) != TaskContinuationOptions::None &&
            status != TaskStatus::Faulted) {
            return false;
        }
        
        if ((options & TaskContinuationOptions::OnlyOnCanceled) != TaskContinuationOptions::None &&
            status != TaskStatus::Canceled) {
            return false;
        }
        
        return true;
    }
};

// Generic Task<T> class
template<typename T>
class Task : public Task {
private:
    std::shared_future<T> typed_future_;
    
public:
    Task() = default;
    
    Task(std::shared_future<T> future, 
         CancellationToken token = CancellationToken::none(),
         TaskCreationOptions options = TaskCreationOptions::None)
        : Task(std::shared_future<void>(future), token, options)
        , typed_future_(std::move(future)) {}
    
    // Get the result
    T get_result() const {
        if (!typed_future_.valid()) {
            throw std::runtime_error("Task has no result");
        }
        
        try {
            auto result = typed_future_.get();
            update_status_from_future();
            return result;
        } catch (...) {
            std::lock_guard<std::mutex> lock(status_mutex_);
            status_ = TaskStatus::Faulted;
            exception_ = std::current_exception();
            throw;
        }
    }
    
    // Continuation methods for Task<T>
    Task continue_with(std::function<void(Task<T>&)> continuation,
                      TaskContinuationOptions options = TaskContinuationOptions::None) {
        return continue_with_impl(std::move(continuation), options);
    }
    
    template<typename TResult>
    Task<TResult> continue_with(std::function<TResult(Task<T>&)> continuation,
                               TaskContinuationOptions options = TaskContinuationOptions::None) {
        return continue_with_impl(std::move(continuation), options);
    }
    
private:
    Task continue_with_impl(std::function<void(Task<T>&)> continuation,
                           TaskContinuationOptions options) {
        auto promise = std::make_shared<std::promise<void>>();
        auto future = promise->get_future().share();
        
        std::thread([this, promise, continuation = std::move(continuation), options]() {
            try {
                // Wait for this task to complete
                this->wait();
                
                // Check continuation options
                if (this->should_run_continuation(options)) {
                    continuation(*this);
                }
                
                promise->set_value();
            } catch (...) {
                promise->set_exception(std::current_exception());
            }
        }).detach();
        
        return Task(std::move(future), cancellation_token_);
    }
    
    template<typename TResult>
    Task<TResult> continue_with_impl(std::function<TResult(Task<T>&)> continuation,
                                    TaskContinuationOptions options) {
        auto promise = std::make_shared<std::promise<TResult>>();
        auto future = promise->get_future().share();
        
        std::thread([this, promise, continuation = std::move(continuation), options]() {
            try {
                // Wait for this task to complete
                this->wait();
                
                // Check continuation options
                if (this->should_run_continuation(options)) {
                    if constexpr (std::is_void_v<TResult>) {
                        continuation(*this);
                        promise->set_value();
                    } else {
                        auto result = continuation(*this);
                        promise->set_value(std::move(result));
                    }
                } else {
                    if constexpr (std::is_void_v<TResult>) {
                        promise->set_value();
                    } else {
                        promise->set_value(TResult{});
                    }
                }
            } catch (...) {
                promise->set_exception(std::current_exception());
            }
        }).detach();
        
        return Task<TResult>(std::move(future), cancellation_token_);
    }
};

} // namespace Threading
} // namespace System
