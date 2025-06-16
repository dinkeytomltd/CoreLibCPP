
#pragma once

#include <mutex>
#include <condition_variable>
#include <atomic>
#include <functional>
#include <stdexcept>

namespace System {
namespace Threading {

class BarrierPostPhaseException : public std::runtime_error {
public:
    explicit BarrierPostPhaseException(const std::string& message) 
        : std::runtime_error(message) {}
};

class Barrier {
private:
    mutable std::mutex mutex_;
    std::condition_variable condition_;
    std::atomic<int> participant_count_;
    std::atomic<int> current_phase_number_;
    int remaining_participants_;
    std::function<void(Barrier&)> post_phase_action_;
    bool disposed_;
    
public:
    explicit Barrier(int participantCount)
        : participant_count_(participantCount)
        , current_phase_number_(0)
        , remaining_participants_(participantCount)
        , disposed_(false) {
        if (participantCount <= 0) {
            throw std::invalid_argument("participantCount must be positive");
        }
    }
    
    Barrier(int participantCount, std::function<void(Barrier&)> postPhaseAction)
        : participant_count_(participantCount)
        , current_phase_number_(0)
        , remaining_participants_(participantCount)
        , post_phase_action_(std::move(postPhaseAction))
        , disposed_(false) {
        if (participantCount <= 0) {
            throw std::invalid_argument("participantCount must be positive");
        }
    }
    
    ~Barrier() {
        dispose();
    }
    
    void signal_and_wait() {
        std::unique_lock<std::mutex> lock(mutex_);
        
        if (disposed_) {
            throw std::runtime_error("Barrier has been disposed");
        }
        
        int current_phase = current_phase_number_.load();
        --remaining_participants_;
        
        if (remaining_participants_ == 0) {
            // Last participant - trigger phase completion
            remaining_participants_ = participant_count_.load();
            current_phase_number_.fetch_add(1);
            
            // Execute post-phase action if provided
            if (post_phase_action_) {
                try {
                    post_phase_action_(*this);
                } catch (...) {
                    condition_.notify_all();
                    throw BarrierPostPhaseException("Post-phase action threw an exception");
                }
            }
            
            condition_.notify_all();
        } else {
            // Wait for phase completion
            condition_.wait(lock, [this, current_phase]() {
                return current_phase_number_.load() > current_phase || disposed_;
            });
            
            if (disposed_) {
                throw std::runtime_error("Barrier has been disposed");
            }
        }
    }
    
    bool signal_and_wait(int millisecondsTimeout) {
        std::unique_lock<std::mutex> lock(mutex_);
        
        if (disposed_) {
            throw std::runtime_error("Barrier has been disposed");
        }
        
        int current_phase = current_phase_number_.load();
        --remaining_participants_;
        
        if (remaining_participants_ == 0) {
            // Last participant - trigger phase completion
            remaining_participants_ = participant_count_.load();
            current_phase_number_.fetch_add(1);
            
            // Execute post-phase action if provided
            if (post_phase_action_) {
                try {
                    post_phase_action_(*this);
                } catch (...) {
                    condition_.notify_all();
                    throw BarrierPostPhaseException("Post-phase action threw an exception");
                }
            }
            
            condition_.notify_all();
            return true;
        } else {
            // Wait for phase completion with timeout
            auto timeout = std::chrono::milliseconds(millisecondsTimeout);
            bool result = condition_.wait_for(lock, timeout, [this, current_phase]() {
                return current_phase_number_.load() > current_phase || disposed_;
            });
            
            if (disposed_) {
                throw std::runtime_error("Barrier has been disposed");
            }
            
            return result;
        }
    }
    
    long add_participant() {
        std::lock_guard<std::mutex> lock(mutex_);
        
        if (disposed_) {
            throw std::runtime_error("Barrier has been disposed");
        }
        
        participant_count_.fetch_add(1);
        ++remaining_participants_;
        
        return participant_count_.load();
    }
    
    void remove_participant() {
        std::lock_guard<std::mutex> lock(mutex_);
        
        if (disposed_) {
            throw std::runtime_error("Barrier has been disposed");
        }
        
        if (participant_count_.load() <= 0) {
            throw std::runtime_error("No participants to remove");
        }
        
        participant_count_.fetch_sub(1);
        --remaining_participants_;
        
        // If this was the last participant needed, complete the phase
        if (remaining_participants_ == 0 && participant_count_.load() > 0) {
            remaining_participants_ = participant_count_.load();
            current_phase_number_.fetch_add(1);
            
            if (post_phase_action_) {
                try {
                    post_phase_action_(*this);
                } catch (...) {
                    condition_.notify_all();
                    throw BarrierPostPhaseException("Post-phase action threw an exception");
                }
            }
            
            condition_.notify_all();
        }
    }
    
    int get_participant_count() const {
        return participant_count_.load();
    }
    
    int get_participants_remaining() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return remaining_participants_;
    }
    
    long get_current_phase_number() const {
        return current_phase_number_.load();
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
