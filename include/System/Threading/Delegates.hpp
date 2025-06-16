
#pragma once

#include <functional>
#include <memory>

namespace System {
namespace Threading {

// Forward declarations
class Task;
template<typename T> class Task;
class CancellationToken;

// Action delegates (void return)
using Action = std::function<void()>;
using ParameterizedThreadStart = std::function<void(void*)>;
using ThreadStart = std::function<void()>;
using TimerCallback = std::function<void(void*)>;
using WaitCallback = std::function<void(void*)>;
using WaitOrTimerCallback = std::function<void(void*, bool)>;

// Function delegates (with return values)
template<typename TResult>
using Func = std::function<TResult()>;

template<typename T, typename TResult>
using Func1 = std::function<TResult(T)>;

template<typename T1, typename T2, typename TResult>
using Func2 = std::function<TResult(T1, T2)>;

template<typename T1, typename T2, typename T3, typename TResult>
using Func3 = std::function<TResult(T1, T2, T3)>;

// Task-related delegates
using TaskAction = std::function<void(Task&)>;

template<typename T>
using TaskAction1 = std::function<void(Task<T>&)>;

template<typename TResult>
using TaskFunc = std::function<TResult(Task&)>;

template<typename T, typename TResult>
using TaskFunc1 = std::function<TResult(Task<T>&)>;

// Continuation delegates
using ContinuationAction = std::function<void(Task&)>;

template<typename T>
using ContinuationAction1 = std::function<void(Task<T>&)>;

template<typename TResult>
using ContinuationFunc = std::function<TResult(Task&)>;

template<typename T, typename TResult>
using ContinuationFunc1 = std::function<TResult(Task<T>&)>;

// Cancellation delegates
using CancellationCallback = std::function<void()>;
using CancellationCallbackWithState = std::function<void(void*)>;

// Synchronization delegates
using SendOrPostCallback = std::function<void(void*)>;

// Exception handling delegates
using UnhandledExceptionEventHandler = std::function<void(void*, std::exception_ptr)>;

// Delegate helper class for managing function pointers and callbacks
template<typename TSignature>
class Delegate;

template<typename TReturn, typename... TArgs>
class Delegate<TReturn(TArgs...)> {
private:
    std::function<TReturn(TArgs...)> function_;
    
public:
    Delegate() = default;
    
    template<typename F>
    Delegate(F&& f) : function_(std::forward<F>(f)) {}
    
    template<typename F>
    Delegate& operator=(F&& f) {
        function_ = std::forward<F>(f);
        return *this;
    }
    
    TReturn operator()(TArgs... args) const {
        if (function_) {
            return function_(args...);
        }
        if constexpr (!std::is_void_v<TReturn>) {
            return TReturn{};
        }
    }
    
    explicit operator bool() const {
        return static_cast<bool>(function_);
    }
    
    bool is_null() const {
        return !function_;
    }
    
    void reset() {
        function_ = nullptr;
    }
    
    // Combine delegates (multicast)
    Delegate operator+(const Delegate& other) const {
        if (!function_) {
            return other;
        }
        if (!other.function_) {
            return *this;
        }
        
        return Delegate([f1 = function_, f2 = other.function_](TArgs... args) -> TReturn {
            if constexpr (std::is_void_v<TReturn>) {
                f1(args...);
                f2(args...);
            } else {
                f1(args...);
                return f2(args...); // Return the result of the last delegate
            }
        });
    }
    
    Delegate& operator+=(const Delegate& other) {
        *this = *this + other;
        return *this;
    }
    
    // Remove delegates
    Delegate operator-(const Delegate& other) const {
        // Simple implementation - in a full implementation, this would need
        // more sophisticated delegate tracking
        if (function_ && other.function_) {
            // For simplicity, just return empty delegate if trying to remove
            return Delegate{};
        }
        return *this;
    }
    
    Delegate& operator-=(const Delegate& other) {
        *this = *this - other;
        return *this;
    }
};

// Specialized delegate types
using ActionDelegate = Delegate<void()>;
using PredicateDelegate = Delegate<bool()>;

template<typename T>
using ActionDelegate1 = Delegate<void(T)>;

template<typename T>
using PredicateDelegate1 = Delegate<bool(T)>;

template<typename TResult>
using FuncDelegate = Delegate<TResult()>;

template<typename T, typename TResult>
using FuncDelegate1 = Delegate<TResult(T)>;

} // namespace Threading
} // namespace System
