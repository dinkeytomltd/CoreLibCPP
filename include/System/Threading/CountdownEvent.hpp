
#pragma once

#include <mutex>
#include <condition_variable>
#include <atomic>
#include <chrono>
#include <stdexcept>

namespace System {
namespace Threading {

class CountdownEvent {
private:
    mutable std::mutex mutex_;
    std::condition_variable condition_;
    std::atomic<int> current_count_;
    int initial_count_;
    bool disposed_;
    
public:
    explicit CountdownEvent(int initialCount)
        : current_count_(initialCount)
        , initial_count_(initialCount)
        , disposed_(false) {
        if (initialCount < 0) {
            throw std::invalid_argument("initialCount cannot be negative");
        }
    }
    
    ~CountdownEvent() {
        dispose();
    }
    
    void wait() {
        std::unique_lock<std::mutex> lock(mutex_);
        condition_.wait(lock, [this]() {
            return current_count_.load() == 0 || disposed_;
        });
        
        if (disposed_) {
            throw std::runtime_error("CountdownEvent has been disposed");
        }
    }
    
    bool wait(int millisecondsTimeout) {
        std::unique_lock<std::mutex> lock(mutex_);
        auto timeout = std::chrono::milliseconds(millisecondsTimeout);
        
        bool result = condition_.wait_for(lock, timeout, [this]() {
            return current_count_.load() == 0 || disposed_;
        });
        
        if (disposed_) {
            throw std::runtime_error("CountdownEvent has been disposed");
        }
        
        return result && current_count_.load() == 0;
    }
    
    template<typename Rep, typename Period>
    bool wait(const std::chrono::duration<Rep, Period>& timeout) {
        std::unique_lock<std::mutex> lock(mutex_);
        
        bool result = condition_.wait_for(lock, timeout, [this]() {
            return current_count_.load() == 0 || disposed_;
        });
        
        if (disposed_) {
            throw std::runtime_error("CountdownEvent has been disposed");
        }
        
        return result && current_count_.load() == 0;
    }
    
    bool signal() {
        std::lock_guard<std::mutex> lock(mutex_);
        
        if (disposed_) {
            throw std::runtime_error("CountdownEvent has been disposed");
        }
        
        if (current_count_.load() <= 0) {
            return false;
        }
        
        int new_count = current_count_.fetch_sub(1) - 1;
        
        if (new_count == 0) {
            condition_.notify_all();
        }
        
        return true;
    }
    
    bool signal(int signalCount) {
        if (signalCount <= 0) {
            throw std::invalid_argument("signalCount must be positive");
        }
        
        std::lock_guard<std::mutex> lock(mutex_);
        
        if (disposed_) {
            throw std::runtime_error("CountdownEvent has been disposed");
        }
        
        int current = current_count_.load();
        if (current < signalCount) {
            return false;
        }
        
        int new_count = current_count_.fetch_sub(signalCount) - signalCount;
        
        if (new_count == 0) {
            condition_.notify_all();
        }
        
        return true;
    }
    
    void add_count() {
        add_count(1);
    }
    
    void add_count(int signalCount) {
        if (signalCount <= 0) {
            throw std::invalid_argument("signalCount must be positive");
        }
        
        std::lock_guard<std::mutex> lock(mutex_);
        
        if (disposed_) {
            throw std::runtime_error("CountdownEvent has been disposed");
        }
        
        if (current_count_.load() == 0) {
            throw std::runtime_error("Cannot add count when CountdownEvent is set");
        }
        
        current_count_.fetch_add(signalCount);
    }
    
    bool try_add_count() {
        return try_add_count(1);
    }
    
    bool try_add_count(int signalCount) {
        if (signalCount <= 0) {
            throw std::invalid_argument("signalCount must be positive");
        }
        
        std::lock_guard<std::mutex> lock(mutex_);
        
        if (disposed_ || current_count_.load() == 0) {
            return false;
        }
        
        current_count_.fetch_add(signalCount);
        return true;
    }
    
    void reset() {
        reset(initial_count_);
    }
    
    void reset(int count) {
        if (count < 0) {
            throw std::invalid_argument("count cannot be negative");
        }
        
        std::lock_guard<std::mutex> lock(mutex_);
        
        if (disposed_) {
            throw std::runtime_error("CountdownEvent has been disposed");
        }
        
        current_count_.store(count);
        initial_count_ = count;
    }
    
    int get_current_count() const {
        return current_count_.load();
    }
    
    int get_initial_count() const {
        return initial_count_;
    }
    
    bool is_set() const {
        return current_count_.load() == 0;
    }
    
    void dispose() {
        std::lock_guard<std::mutex> lock(mutex_);
        if (!disposed_) {
            disposed_ = true;
            condition_.notify_all();
        }
    }
};

} // namespace Threading
} // namespace System
