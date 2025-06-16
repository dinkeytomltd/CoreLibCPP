
#pragma once

#include <coroutine>
#include <memory>

namespace System::Threading::Tasks
{
    // Forward declarations
    class SynchronizationContext;

    // YieldAwaitable struct for Task.Yield() implementation
    struct YieldAwaitable
    {
        struct YieldAwaiter
        {
            bool await_ready() const noexcept
            {
                // Always yield - never complete synchronously
                return false;
            }

            void await_suspend(std::coroutine_handle<> handle) const noexcept
            {
                // Schedule the continuation to run asynchronously
                ScheduleContinuation(handle);
            }

            void await_resume() const noexcept
            {
                // Nothing to return for void yield
            }

        private:
            void ScheduleContinuation(std::coroutine_handle<> handle) const;
        };

        YieldAwaiter operator co_await() const noexcept
        {
            return YieldAwaiter{};
        }
    };
}
