
#pragma once

#include <shared_mutex>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <chrono>
#include <thread>
#include <stdexcept>

namespace System {
namespace Threading {

enum class LockRecursionPolicy {
    NoRecursion,
    SupportsRecursion
};

class LockRecursionException : public std::runtime_error {
public:
    explicit LockRecursionException(const std::string& message) 
        : std::runtime_error(message) {}
};

class ReaderWriterLockSlim {
private:
    mutable std::shared_mutex shared_mutex_;
    mutable std::mutex state_mutex_;
    std::condition_variable condition_;
    
    LockRecursionPolicy recursion_policy_;
    std::atomic<int> reader_count_;
    std::atomic<bool> writer_held_;
    std::atomic<bool> upgradeable_held_;
    std::atomic<std::thread::id> writer_thread_id_;
    std::atomic<std::thread::id> upgradeable_thread_id_;
    std::atomic<int> waiting_writers_;
    std::atomic<int> waiting_upgraders_;
    bool disposed_;
    
    // Recursion tracking (only used when recursion is supported)
    thread_local static int reader_recursion_count_;
    thread_local static int writer_recursion_count_;
    thread_local static int upgradeable_recursion_count_;
    
public:
    explicit ReaderWriterLockSlim(LockRecursionPolicy recursionPolicy = LockRecursionPolicy::NoRecursion)
        : recursion_policy_(recursionPolicy)
        , reader_count_(0)
        , writer_held_(false)
        , upgradeable_held_(false)
        , writer_thread_id_(std::thread::id{})
        , upgradeable_thread_id_(std::thread::id{})
        , waiting_writers_(0)
        , waiting_upgraders_(0)
        , disposed_(false) {}
    
    ~ReaderWriterLockSlim() {
        dispose();
    }
    
    void enter_read_lock() {
        if (disposed_) {
            throw std::runtime_error("ReaderWriterLockSlim has been disposed");
        }
        
        std::thread::id current_thread = std::this_thread::get_id();
        
        // Check for recursion
        if (recursion_policy_ == LockRecursionPolicy::NoRecursion) {
            if (writer_thread_id_.load() == current_thread || 
                upgradeable_thread_id_.load() == current_thread) {
                throw LockRecursionException("Read lock cannot be acquired when write or upgradeable lock is held by the same thread");
            }
        } else {
            if (writer_thread_id_.load() == current_thread) {
                ++writer_recursion_count_;
                return;
            }
            if (upgradeable_thread_id_.load() == current_thread) {
                ++upgradeable_recursion_count_;
                return;
            }
            if (reader_recursion_count_ > 0) {
                ++reader_recursion_count_;
                return;
            }
        }
        
        std::unique_lock<std::mutex> lock(state_mutex_);
        condition_.wait(lock, [this]() {
            return !writer_held_.load() && waiting_upgraders_.load() == 0;
        });
        
        reader_count_.fetch_add(1);
        if (recursion_policy_ == LockRecursionPolicy::SupportsRecursion) {
            ++reader_recursion_count_;
        }
    }
    
    bool try_enter_read_lock() {
        if (disposed_) {
            throw std::runtime_error("ReaderWriterLockSlim has been disposed");
        }
        
        std::thread::id current_thread = std::this_thread::get_id();
        
        // Check for recursion
        if (recursion_policy_ == LockRecursionPolicy::SupportsRecursion) {
            if (writer_thread_id_.load() == current_thread) {
                ++writer_recursion_count_;
                return true;
            }
            if (upgradeable_thread_id_.load() == current_thread) {
                ++upgradeable_recursion_count_;
                return true;
            }
            if (reader_recursion_count_ > 0) {
                ++reader_recursion_count_;
                return true;
            }
        }
        
        std::unique_lock<std::mutex> lock(state_mutex_, std::try_to_lock);
        if (!lock.owns_lock()) {
            return false;
        }
        
        if (writer_held_.load() || waiting_upgraders_.load() > 0) {
            return false;
        }
        
        reader_count_.fetch_add(1);
        if (recursion_policy_ == LockRecursionPolicy::SupportsRecursion) {
            ++reader_recursion_count_;
        }
        return true;
    }
    
    void exit_read_lock() {
        if (disposed_) {
            throw std::runtime_error("ReaderWriterLockSlim has been disposed");
        }
        
        if (recursion_policy_ == LockRecursionPolicy::SupportsRecursion) {
            std::thread::id current_thread = std::this_thread::get_id();
            
            if (writer_thread_id_.load() == current_thread && writer_recursion_count_ > 0) {
                --writer_recursion_count_;
                return;
            }
            if (upgradeable_thread_id_.load() == current_thread && upgradeable_recursion_count_ > 0) {
                --upgradeable_recursion_count_;
                return;
            }
            if (reader_recursion_count_ > 0) {
                --reader_recursion_count_;
                if (reader_recursion_count_ > 0) {
                    return;
                }
            }
        }
        
        std::lock_guard<std::mutex> lock(state_mutex_);
        
        if (reader_count_.load() <= 0) {
            throw std::runtime_error("No read lock to exit");
        }
        
        reader_count_.fetch_sub(1);
        
        if (reader_count_.load() == 0) {
            condition_.notify_all();
        }
    }
    
    void enter_write_lock() {
        if (disposed_) {
            throw std::runtime_error("ReaderWriterLockSlim has been disposed");
        }
        
        std::thread::id current_thread = std::this_thread::get_id();
        
        // Check for recursion
        if (recursion_policy_ == LockRecursionPolicy::NoRecursion) {
            if (writer_thread_id_.load() == current_thread || 
                upgradeable_thread_id_.load() == current_thread ||
                reader_recursion_count_ > 0) {
                throw LockRecursionException("Write lock cannot be acquired when any lock is held by the same thread");
            }
        } else {
            if (writer_thread_id_.load() == current_thread) {
                ++writer_recursion_count_;
                return;
            }
        }
        
        waiting_writers_.fetch_add(1);
        
        std::unique_lock<std::mutex> lock(state_mutex_);
        condition_.wait(lock, [this]() {
            return !writer_held_.load() && reader_count_.load() == 0 && !upgradeable_held_.load();
        });
        
        waiting_writers_.fetch_sub(1);
        writer_held_.store(true);
        writer_thread_id_.store(current_thread);
        
        if (recursion_policy_ == LockRecursionPolicy::SupportsRecursion) {
            ++writer_recursion_count_;
        }
    }
    
    bool try_enter_write_lock() {
        if (disposed_) {
            throw std::runtime_error("ReaderWriterLockSlim has been disposed");
        }
        
        std::thread::id current_thread = std::this_thread::get_id();
        
        // Check for recursion
        if (recursion_policy_ == LockRecursionPolicy::SupportsRecursion) {
            if (writer_thread_id_.load() == current_thread) {
                ++writer_recursion_count_;
                return true;
            }
        }
        
        std::unique_lock<std::mutex> lock(state_mutex_, std::try_to_lock);
        if (!lock.owns_lock()) {
            return false;
        }
        
        if (writer_held_.load() || reader_count_.load() > 0 || upgradeable_held_.load()) {
            return false;
        }
        
        writer_held_.store(true);
        writer_thread_id_.store(current_thread);
        
        if (recursion_policy_ == LockRecursionPolicy::SupportsRecursion) {
            ++writer_recursion_count_;
        }
        return true;
    }
    
    void exit_write_lock() {
        if (disposed_) {
            throw std::runtime_error("ReaderWriterLockSlim has been disposed");
        }
        
        std::thread::id current_thread = std::this_thread::get_id();
        
        if (writer_thread_id_.load() != current_thread) {
            throw std::runtime_error("Write lock not held by current thread");
        }
        
        if (recursion_policy_ == LockRecursionPolicy::SupportsRecursion) {
            if (writer_recursion_count_ > 1) {
                --writer_recursion_count_;
                return;
            }
            writer_recursion_count_ = 0;
        }
        
        std::lock_guard<std::mutex> lock(state_mutex_);
        
        writer_held_.store(false);
        writer_thread_id_.store(std::thread::id{});
        condition_.notify_all();
    }
    
    void enter_upgradeable_read_lock() {
        if (disposed_) {
            throw std::runtime_error("ReaderWriterLockSlim has been disposed");
        }
        
        std::thread::id current_thread = std::this_thread::get_id();
        
        // Check for recursion
        if (recursion_policy_ == LockRecursionPolicy::NoRecursion) {
            if (writer_thread_id_.load() == current_thread || 
                upgradeable_thread_id_.load() == current_thread) {
                throw LockRecursionException("Upgradeable read lock cannot be acquired when write or upgradeable lock is held by the same thread");
            }
        } else {
            if (upgradeable_thread_id_.load() == current_thread) {
                ++upgradeable_recursion_count_;
                return;
            }
            if (writer_thread_id_.load() == current_thread) {
                ++writer_recursion_count_;
                return;
            }
        }
        
        std::unique_lock<std::mutex> lock(state_mutex_);
        condition_.wait(lock, [this]() {
            return !writer_held_.load() && !upgradeable_held_.load();
        });
        
        upgradeable_held_.store(true);
        upgradeable_thread_id_.store(current_thread);
        reader_count_.fetch_add(1);
        
        if (recursion_policy_ == LockRecursionPolicy::SupportsRecursion) {
            ++upgradeable_recursion_count_;
        }
    }
    
    bool try_enter_upgradeable_read_lock() {
        if (disposed_) {
            throw std::runtime_error("ReaderWriterLockSlim has been disposed");
        }
        
        std::thread::id current_thread = std::this_thread::get_id();
        
        // Check for recursion
        if (recursion_policy_ == LockRecursionPolicy::SupportsRecursion) {
            if (upgradeable_thread_id_.load() == current_thread) {
                ++upgradeable_recursion_count_;
                return true;
            }
            if (writer_thread_id_.load() == current_thread) {
                ++writer_recursion_count_;
                return true;
            }
        }
        
        std::unique_lock<std::mutex> lock(state_mutex_, std::try_to_lock);
        if (!lock.owns_lock()) {
            return false;
        }
        
        if (writer_held_.load() || upgradeable_held_.load()) {
            return false;
        }
        
        upgradeable_held_.store(true);
        upgradeable_thread_id_.store(current_thread);
        reader_count_.fetch_add(1);
        
        if (recursion_policy_ == LockRecursionPolicy::SupportsRecursion) {
            ++upgradeable_recursion_count_;
        }
        return true;
    }
    
    void exit_upgradeable_read_lock() {
        if (disposed_) {
            throw std::runtime_error("ReaderWriterLockSlim has been disposed");
        }
        
        std::thread::id current_thread = std::this_thread::get_id();
        
        if (recursion_policy_ == LockRecursionPolicy::SupportsRecursion) {
            if (writer_thread_id_.load() == current_thread && writer_recursion_count_ > 0) {
                --writer_recursion_count_;
                return;
            }
            if (upgradeable_thread_id_.load() == current_thread && upgradeable_recursion_count_ > 1) {
                --upgradeable_recursion_count_;
                return;
            }
        }
        
        if (upgradeable_thread_id_.load() != current_thread) {
            throw std::runtime_error("Upgradeable read lock not held by current thread");
        }
        
        std::lock_guard<std::mutex> lock(state_mutex_);
        
        upgradeable_held_.store(false);
        upgradeable_thread_id_.store(std::thread::id{});
        reader_count_.fetch_sub(1);
        
        if (recursion_policy_ == LockRecursionPolicy::SupportsRecursion) {
            upgradeable_recursion_count_ = 0;
        }
        
        condition_.notify_all();
    }
    
    // Properties
    bool is_read_lock_held() const {
        if (recursion_policy_ == LockRecursionPolicy::SupportsRecursion) {
            return reader_recursion_count_ > 0 || 
                   writer_thread_id_.load() == std::this_thread::get_id() ||
                   upgradeable_thread_id_.load() == std::this_thread::get_id();
        }
        return reader_count_.load() > 0;
    }
    
    bool is_write_lock_held() const {
        return writer_thread_id_.load() == std::this_thread::get_id();
    }
    
    bool is_upgradeable_read_lock_held() const {
        if (recursion_policy_ == LockRecursionPolicy::SupportsRecursion) {
            return upgradeable_thread_id_.load() == std::this_thread::get_id() ||
                   writer_thread_id_.load() == std::this_thread::get_id();
        }
        return upgradeable_thread_id_.load() == std::this_thread::get_id();
    }
    
    int get_current_read_count() const {
        return reader_count_.load();
    }
    
    int get_waiting_read_count() const {
        // This is an approximation since we don't track waiting readers separately
        return 0;
    }
    
    int get_waiting_write_count() const {
        return waiting_writers_.load();
    }
    
    int get_waiting_upgrade_count() const {
        return waiting_upgraders_.load();
    }
    
    LockRecursionPolicy get_recursion_policy() const {
        return recursion_policy_;
    }
    
    void dispose() {
        std::lock_guard<std::mutex> lock(state_mutex_);
        if (!disposed_) {
            disposed_ = true;
            condition_.notify_all();
        }
    }
};

// Thread-local storage for recursion tracking
thread_local int ReaderWriterLockSlim::reader_recursion_count_ = 0;
thread_local int ReaderWriterLockSlim::writer_recursion_count_ = 0;
thread_local int ReaderWriterLockSlim::upgradeable_recursion_count_ = 0;

} // namespace Threading
} // namespace System
