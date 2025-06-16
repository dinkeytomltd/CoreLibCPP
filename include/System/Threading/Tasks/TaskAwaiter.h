
#pragma once

#include "Task.h"
#include <coroutine>
#include <functional>

namespace System::Threading::Tasks
{
    struct TaskAwaiter
    {
    private:
        std::shared_ptr<Task> task;

    public:
        TaskAwaiter(std::shared_ptr<Task> task);

        bool await_ready() const noexcept;
        void await_suspend(std::coroutine_handle<> handle);
        void await_resume();
    };

    template<typename T>
    struct TaskAwaiter
    {
    private:
        std::shared_ptr<Task<T>> task;

    public:
        TaskAwaiter(std::shared_ptr<Task<T>> task);

        bool await_ready() const noexcept;
        void await_suspend(std::coroutine_handle<> handle);
        T await_resume();
    };

    struct ConfiguredTaskAwaitable
    {
        struct ConfiguredTaskAwaiter
        {
            std::shared_ptr<Task> task;
            bool continueOnCapturedContext;

            bool await_ready() const noexcept;
            void await_suspend(std::coroutine_handle<> handle);
            void await_resume();
        };

        ConfiguredTaskAwaiter awaiter;
        ConfiguredTaskAwaiter GetAwaiter() { return awaiter; }
    };

    template<typename T>
    struct ConfiguredTaskAwaitable
    {
        struct ConfiguredTaskAwaiter
        {
            std::shared_ptr<Task<T>> task;
            bool continueOnCapturedContext;

            bool await_ready() const noexcept;
            void await_suspend(std::coroutine_handle<> handle);
            T await_resume();
        };

        ConfiguredTaskAwaiter awaiter;
        ConfiguredTaskAwaiter GetAwaiter() { return awaiter; }
    };
}
