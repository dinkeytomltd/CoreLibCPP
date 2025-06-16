
#pragma once

#include "System/Threading/SynchronizationContext.h"
#include "System/Threading/Tasks/Task.h"

namespace System {
namespace Threading {

template<typename TResult = void>
class AsyncMethodBuilder {
public:
    AsyncMethodBuilder();

    // Create the task
    std::shared_ptr<Tasks::Task<TResult>> Task();

    // Set result
    void SetResult(const TResult& result);
    void SetException(std::exception_ptr exception);

    // Await support with context capture
    template<typename TAwaiter>
    void AwaitOnCompleted(TAwaiter& awaiter, std::function<void()> continuation);

    template<typename TAwaiter>
    void AwaitUnsafeOnCompleted(TAwaiter& awaiter, std::function<void()> continuation);

private:
    std::shared_ptr<Tasks::Task<TResult>> m_task;
    std::shared_ptr<SynchronizationContext> m_capturedContext;
};

// Specialization for void
template<>
class AsyncMethodBuilder<void> {
public:
    AsyncMethodBuilder();

    std::shared_ptr<Tasks::Task> Task();
    void SetResult();
    void SetException(std::exception_ptr exception);

    template<typename TAwaiter>
    void AwaitOnCompleted(TAwaiter& awaiter, std::function<void()> continuation);

private:
    std::shared_ptr<Tasks::Task> m_task;
    std::shared_ptr<SynchronizationContext> m_capturedContext;
};

} // namespace Threading
} // namespace System
