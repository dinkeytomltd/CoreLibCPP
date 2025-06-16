
#pragma once

#include "Structures.hpp"
#include "Enums.hpp"
#include "Delegates.hpp"
#include <memory>
#include <vector>
#include <mutex>
#include <atomic>
#include <exception>
#include <stdexcept>

namespace System {
namespace Threading {

class OperationCanceledException : public std::runtime_error {
public:
    explicit OperationCanceledException(const std::string& message = "The operation was canceled")
        : std::runtime_error(message) {}
};

class CancellationTokenSource;

class CancellationToken {
private:
    struct CallbackRegistration {
        CancellationCallback callback;
        void* state;
        bool execute_synchronously;
        
        CallbackRegistration(CancellationCallback cb, void* st, bool sync)
            : callback(std::move(cb)), state(st), execute_synchronously(sync) {}
    };
    
    std::shared_ptr<std::atomic<bool>> is_canceled_;
    std::shared_ptr<std::mutex> callbacks_mutex_;
    std::shared_ptr<std::vector<std::shared_ptr<CallbackRegistration>>> callbacks_;
    
    friend class CancellationTokenSource;
    
    CancellationToken(std::shared_ptr<std::atomic<bool>> canceled,
                     std::shared_ptr<std::mutex> mutex,
                     std::shared_ptr<std::vector<std::shared_ptr<CallbackRegistration>>> callbacks)
        : is_canceled_(std::move(canceled))
        , callbacks_mutex_(std::move(mutex))
        , callbacks_(std::move(callbacks)) {}
    
public:
    // Default constructor creates a token that will never be canceled
    CancellationToken() 
        : is_canceled_(std::make_shared<std::atomic<bool>>(false))
        , callbacks_mutex_(std::make_shared<std::mutex>())
        , callbacks_(std::make_shared<std::vector<std::shared_ptr<CallbackRegistration>>>()) {}
    
    // Constructor for pre-canceled token
    explicit CancellationToken(bool canceled)
        : is_canceled_(std::make_shared<std::atomic<bool>>(canceled))
        , callbacks_mutex_(std::make_shared<std::mutex>())
        , callbacks_(std::make_shared<std::vector<std::shared_ptr<CallbackRegistration>>>()) {}
    
    // Copy constructor and assignment
    CancellationToken(const CancellationToken&) = default;
    CancellationToken& operator=(const CancellationToken&) = default;
    
    // Move constructor and assignment
    CancellationToken(CancellationToken&&) = default;
    CancellationToken& operator=(CancellationToken&&) = default;
    
    bool is_cancellation_requested() const {
        return is_canceled_->load();
    }
    
    bool can_be_canceled() const {
        return true; // All tokens can potentially be canceled
    }
    
    void throw_if_cancellation_requested() const {
        if (is_cancellation_requested()) {
            throw OperationCanceledException();
        }
    }
    
    CancellationTokenRegistration register_callback(CancellationCallback callback) {
        return register_callback(std::move(callback), nullptr, false);
    }
    
    CancellationTokenRegistration register_callback(CancellationCallback callback, void* state) {
        return register_callback(std::move(callback), state, false);
    }
    
    CancellationTokenRegistration register_callback(CancellationCallback callback, 
                                                   void* state, 
                                                   bool use_synchronization_context) {
        if (!callback) {
            return CancellationTokenRegistration{};
        }
        
        auto registration = std::make_shared<CallbackRegistration>(
            std::move(callback), state, use_synchronization_context);
        
        {
            std::lock_guard<std::mutex> lock(*callbacks_mutex_);
            
            // If already canceled, execute callback immediately
            if (is_canceled_->load()) {
                try {
                    registration->callback();
                } catch (...) {
                    // Ignore exceptions in callbacks
                }
                return CancellationTokenRegistration{};
            }
            
            callbacks_->push_back(registration);
        }
        
        return CancellationTokenRegistration(registration, 
                                           std::weak_ptr<CancellationToken>(
                                               std::shared_ptr<CancellationToken>(
                                                   const_cast<CancellationToken*>(this), 
                                                   [](CancellationToken*){})));
    }
    
    // Static factory methods
    static CancellationToken none() {
        static CancellationToken never_canceled(false);
        return never_canceled;
    }
    
    static CancellationToken canceled() {
        return CancellationToken(true);
    }
    
    // Equality operators
    bool operator==(const CancellationToken& other) const {
        return is_canceled_ == other.is_canceled_;
    }
    
    bool operator!=(const CancellationToken& other) const {
        return !(*this == other);
    }
    
private:
    void execute_callbacks() {
        std::lock_guard<std::mutex> lock(*callbacks_mutex_);
        
        for (auto& registration : *callbacks_) {
            if (registration) {
                try {
                    registration->callback();
                } catch (...) {
                    // Ignore exceptions in callbacks
                }
            }
        }
        
        callbacks_->clear();
    }
    
    void remove_callback(std::shared_ptr<CallbackRegistration> registration) {
        std::lock_guard<std::mutex> lock(*callbacks_mutex_);
        
        auto it = std::find(callbacks_->begin(), callbacks_->end(), registration);
        if (it != callbacks_->end()) {
            callbacks_->erase(it);
        }
    }
    
    friend struct CancellationTokenRegistration;
};

class CancellationTokenSource {
private:
    CancellationToken token_;
    std::atomic<bool> disposed_;
    
public:
    CancellationTokenSource() 
        : token_(std::make_shared<std::atomic<bool>>(false),
                std::make_shared<std::mutex>(),
                std::make_shared<std::vector<std::shared_ptr<CancellationToken::CallbackRegistration>>>())
        , disposed_(false) {}
    
    ~CancellationTokenSource() {
        dispose();
    }
    
    // Disable copy constructor and assignment
    CancellationTokenSource(const CancellationTokenSource&) = delete;
    CancellationTokenSource& operator=(const CancellationTokenSource&) = delete;
    
    // Move constructor and assignment
    CancellationTokenSource(CancellationTokenSource&& other) noexcept
        : token_(std::move(other.token_))
        , disposed_(other.disposed_.load()) {
        other.disposed_.store(true);
    }
    
    CancellationTokenSource& operator=(CancellationTokenSource&& other) noexcept {
        if (this != &other) {
            dispose();
            token_ = std::move(other.token_);
            disposed_.store(other.disposed_.load());
            other.disposed_.store(true);
        }
        return *this;
    }
    
    CancellationToken get_token() {
        if (disposed_.load()) {
            throw std::runtime_error("CancellationTokenSource has been disposed");
        }
        return token_;
    }
    
    void cancel() {
        if (disposed_.load()) {
            throw std::runtime_error("CancellationTokenSource has been disposed");
        }
        
        bool expected = false;
        if (token_.is_canceled_->compare_exchange_strong(expected, true)) {
            token_.execute_callbacks();
        }
    }
    
    void cancel(bool throw_on_first_exception) {
        if (throw_on_first_exception) {
            cancel();
        } else {
            try {
                cancel();
            } catch (...) {
                // Ignore exceptions if throw_on_first_exception is false
            }
        }
    }
    
    bool is_cancellation_requested() const {
        return !disposed_.load() && token_.is_cancellation_requested();
    }
    
    void dispose() {
        if (!disposed_.exchange(true)) {
            // Mark as disposed but don't cancel automatically
            // The token remains valid but the source cannot be used to cancel it
        }
    }
    
    // Static factory methods
    static CancellationTokenSource create_linked_token_source(const CancellationToken& token) {
        CancellationTokenSource source;
        
        if (token.is_cancellation_requested()) {
            source.cancel();
        } else {
            // Register callback to cancel this source when the linked token is canceled
            token.register_callback([&source]() {
                if (!source.disposed_.load()) {
                    source.cancel();
                }
            });
        }
        
        return source;
    }
    
    template<typename... Tokens>
    static CancellationTokenSource create_linked_token_source(const CancellationToken& token1, 
                                                             const CancellationToken& token2,
                                                             const Tokens&... tokens) {
        CancellationTokenSource source;
        
        auto register_token = [&source](const CancellationToken& token) {
            if (token.is_cancellation_requested()) {
                source.cancel();
            } else {
                token.register_callback([&source]() {
                    if (!source.disposed_.load()) {
                        source.cancel();
                    }
                });
            }
        };
        
        register_token(token1);
        register_token(token2);
        (register_token(tokens), ...);
        
        return source;
    }
};

// Implementation of CancellationTokenRegistration::dispose()
inline void CancellationTokenRegistration::dispose() {
    if (registration_handle_ && !token_.expired()) {
        if (auto token = token_.lock()) {
            auto registration = std::static_pointer_cast<CancellationToken::CallbackRegistration>(registration_handle_);
            token->remove_callback(registration);
        }
    }
    registration_handle_.reset();
    token_.reset();
}

} // namespace Threading
} // namespace System
