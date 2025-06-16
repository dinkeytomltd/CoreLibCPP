
#pragma once

#include "System/Object.h"
#include "System/TimeSpan.h"
#include "TaskStatus.h"
#include "TaskCreationOptions.h"
#include "TaskContinuationOptions.h"
#include <functional>
#include <memory>
#include <vector>
#include <future>
#include <coroutine>

namespace System::Threading {
    class SynchronizationContext;
}

namespace System::Threading
{
    struct CancellationToken;
    class CancellationTokenSource;
}

namespace System::Threading::Tasks
{
    class TaskScheduler;
    struct TaskAwaiter;
    struct ConfiguredTaskAwaitable;
    struct YieldAwaitable;
    template<typename T> class Task;
    class Task : public System::Object
    {
    protected:
        std::shared_ptr<std::promise<void>> promise;
        std::shared_future<void> future;
        TaskStatus status;
        std::exception_ptr exception;
        std::vector<std::shared_ptr<Task>> continuations;
        int id;
        static std::atomic<int> nextTaskId;
        
        // SynchronizationContext support
        std::shared_ptr<System::Threading::SynchronizationContext> m_capturedContext;

    public:
        Task();
        Task(std::function<void()> action);
        Task(std::function<void(System::Object*)> action, System::Object* state);
        virtual ~Task() = default;

        // Execution control
        void Start();
        void Wait();
        bool Wait(int millisecondsTimeout);
        bool Wait(const System::Threading::CancellationToken& cancellationToken);
        bool Wait(int millisecondsTimeout, const System::Threading::CancellationToken& cancellationToken);

        // Properties
        TaskStatus GetStatus() const;
        bool GetIsCompleted() const;
        bool GetIsCompletedSuccessfully() const;
        bool GetIsFaulted() const;
        bool GetIsCanceled() const;
        System::Exception* GetException() const;
        int GetId() const;

        // Continuations
        std::shared_ptr<Task> ContinueWith(std::function<void(std::shared_ptr<Task>)> continuationAction);
        std::shared_ptr<Task> ContinueWith(std::function<void(std::shared_ptr<Task>)> continuationAction, const System::Threading::CancellationToken& cancellationToken);
        std::shared_ptr<Task> ContinueWith(std::function<void(std::shared_ptr<Task>)> continuationAction, TaskContinuationOptions continuationOptions);
        
        // Context-aware continuations
        template<typename TResult>
        std::shared_ptr<Task<TResult>> ContinueWith(
            std::function<TResult(std::shared_ptr<Task>)> continuationFunction,
            std::shared_ptr<System::Threading::SynchronizationContext> context = nullptr);

        template<typename TResult>
        std::shared_ptr<Task<TResult>> ContinueWith(
            std::function<TResult(std::shared_ptr<Task>)> continuationFunction,
            TaskContinuationOptions continuationOptions,
            std::shared_ptr<System::Threading::SynchronizationContext> context = nullptr);

        // Context capture methods
        static void CaptureContext();
        std::shared_ptr<System::Threading::SynchronizationContext> GetCapturedContext() const;

        // Static factory methods
        static std::shared_ptr<Task> Run(std::function<void()> action);
        static std::shared_ptr<Task> Run(std::function<void()> action, const System::Threading::CancellationToken& cancellationToken);
        static std::shared_ptr<Task> Delay(int millisecondsDelay);
        static std::shared_ptr<Task> Delay(const TimeSpan& delay);
        static std::shared_ptr<Task> CompletedTask();
        static std::shared_ptr<Task> FromException(System::Exception* exception);
        static std::shared_ptr<Task> FromCanceled(const System::Threading::CancellationToken& cancellationToken);
        
        // Yield method for async/await patterns
        static YieldAwaitable Yield();

        // Combinators
        static std::shared_ptr<Task> WhenAll(const std::vector<std::shared_ptr<Task>>& tasks);
        static std::shared_ptr<Task> WhenAny(const std::vector<std::shared_ptr<Task>>& tasks);

        // Awaiter support
        TaskAwaiter GetAwaiter();
        ConfiguredTaskAwaitable ConfigureAwait(bool continueOnCapturedContext);

    protected:
        void SetStatus(TaskStatus newStatus);
        void SetException(std::exception_ptr ex);
        
        // Helper method for context-aware continuation
        void ScheduleContinuation(std::function<void()> continuation, 
                                std::shared_ptr<System::Threading::SynchronizationContext> context);

    public:
        // Methods needed by TaskCompletionSource
        void SetAsyncState(System::Object* state);
        System::Object* GetAsyncState() const;
        void SetRunContinuationsAsynchronously(bool value);
        void SetCancellationToken(const System::Threading::CancellationToken& token);
        void CompletePromise();
        void CompletePromiseWithException(std::exception_ptr ex);
        void CompletePromiseWithCancellation();

    private:
        System::Object* m_asyncState = nullptr;
        bool m_runContinuationsAsynchronously = false;
        System::Threading::CancellationToken m_cancellationToken;
    };

    template<typename T>
    class Task : public System::Threading::Tasks::Task
    {
    private:
        std::shared_ptr<std::promise<T>> typedPromise;
        std::shared_future<T> typedFuture;
        T result;

    public:
        Task();
        Task(std::function<T()> function);
        Task(std::function<T(System::Object*)> function, System::Object* state);

        // Result access
        T GetResult();
        T GetResult(const System::Threading::CancellationToken& cancellationToken);

        // Continuations with result
        template<typename TResult>
        std::shared_ptr<Task<TResult>> ContinueWith(std::function<TResult(std::shared_ptr<Task<T>>)> continuationFunction);

        std::shared_ptr<Task> ContinueWith(std::function<void(std::shared_ptr<Task<T>>)> continuationAction);

        // Static factory methods
        static std::shared_ptr<Task<T>> Run(std::function<T()> function);
        static std::shared_ptr<Task<T>> Run(std::function<T()> function, const System::Threading::CancellationToken& cancellationToken);
        static std::shared_ptr<Task<T>> FromResult(T result);

        // Awaiter support
        TaskAwaiter<T> GetAwaiter();
        ConfiguredTaskAwaitable<T> ConfigureAwait(bool continueOnCapturedContext);

        // Methods needed by TaskCompletionSource
        void SetResultInternal(const T& result);
    };
}
