
#pragma once

#include <chrono>
#include <thread>
#include <atomic>
#include <functional>

namespace System {
namespace Threading {

struct SpinWait {
private:
    int spin_count_;
    static constexpr int YIELD_THRESHOLD = 10;
    static constexpr int SLEEP_0_EVERY_HOW_MANY_TIMES = 5;
    static constexpr int SLEEP_1_EVERY_HOW_MANY_TIMES = 20;
    
public:
    SpinWait() : spin_count_(0) {}
    
    void spin_once() {
        if (next_spin_will_yield()) {
            int yieldsSoFar = (spin_count_ >= YIELD_THRESHOLD ? spin_count_ - YIELD_THRESHOLD : spin_count_);
            
            if ((yieldsSoFar % SLEEP_1_EVERY_HOW_MANY_TIMES) == (SLEEP_1_EVERY_HOW_MANY_TIMES - 1)) {
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
            } else if ((yieldsSoFar % SLEEP_0_EVERY_HOW_MANY_TIMES) == (SLEEP_0_EVERY_HOW_MANY_TIMES - 1)) {
                std::this_thread::sleep_for(std::chrono::milliseconds(0));
            } else {
                std::this_thread::yield();
            }
        } else {
            // Busy wait with exponential backoff
            int iterations = 4 << spin_count_;
            for (int i = 0; i < iterations; ++i) {
                std::atomic_thread_fence(std::memory_order_acquire);
            }
        }
        
        spin_count_ = (spin_count_ == INT_MAX) ? YIELD_THRESHOLD : spin_count_ + 1;
    }
    
    bool next_spin_will_yield() const {
        return spin_count_ >= YIELD_THRESHOLD;
    }
    
    int get_count() const {
        return spin_count_;
    }
    
    void reset() {
        spin_count_ = 0;
    }
    
    // Static utility methods
    static void spin_until(const std::function<bool()>& condition) {
        SpinWait spinner;
        while (!condition()) {
            spinner.spin_once();
        }
    }
    
    template<typename Rep, typename Period>
    static bool spin_until(const std::function<bool()>& condition, 
                          const std::chrono::duration<Rep, Period>& timeout) {
        SpinWait spinner;
        auto start_time = std::chrono::steady_clock::now();
        
        while (!condition()) {
            if (std::chrono::steady_clock::now() - start_time >= timeout) {
                return false;
            }
            spinner.spin_once();
        }
        return true;
    }
};

// Forward declaration for CancellationTokenRegistration
class CancellationToken;

struct CancellationTokenRegistration {
private:
    std::shared_ptr<void> registration_handle_;
    std::weak_ptr<CancellationToken> token_;
    
public:
    CancellationTokenRegistration() = default;
    
    CancellationTokenRegistration(std::shared_ptr<void> handle, 
                                 std::weak_ptr<CancellationToken> token)
        : registration_handle_(std::move(handle))
        , token_(std::move(token)) {}
    
    // Move constructor and assignment
    CancellationTokenRegistration(CancellationTokenRegistration&& other) noexcept
        : registration_handle_(std::move(other.registration_handle_))
        , token_(std::move(other.token_)) {}
    
    CancellationTokenRegistration& operator=(CancellationTokenRegistration&& other) noexcept {
        if (this != &other) {
            dispose();
            registration_handle_ = std::move(other.registration_handle_);
            token_ = std::move(other.token_);
        }
        return *this;
    }
    
    // Disable copy constructor and assignment
    CancellationTokenRegistration(const CancellationTokenRegistration&) = delete;
    CancellationTokenRegistration& operator=(const CancellationTokenRegistration&) = delete;
    
    ~CancellationTokenRegistration() {
        dispose();
    }
    
    void dispose();
    
    bool operator==(const CancellationTokenRegistration& other) const {
        return registration_handle_ == other.registration_handle_;
    }
    
    bool operator!=(const CancellationTokenRegistration& other) const {
        return !(*this == other);
    }
    
    // Check if the registration is valid
    bool is_valid() const {
        return registration_handle_ != nullptr && !token_.expired();
    }
};

} // namespace Threading
} // namespace System
