
#pragma once

#include "System/Object.h"
#include "Task.h"
#include "TaskCreationOptions.h"
#include "System/Threading/CancellationToken.h"
#include "System/Exception.h"
#include <memory>
#include <atomic>
#include <mutex>
#include <vector>

namespace System::Threading::Tasks
{
    template<typename TResult>
    class TaskCompletionSource : public System::Object
    {
    private:
        std::shared_ptr<Task<TResult>> m_task;
        std::atomic<bool> m_completed{false};
        mutable std::mutex m_mutex;

    public:
        // Constructors
        TaskCompletionSource()
            : m_task(std::make_shared<Task<TResult>>())
        {
            // Task starts in Created state, we'll manage its completion manually
        }

        explicit TaskCompletionSource(TaskCreationOptions creationOptions)
            : m_task(std::make_shared<Task<TResult>>())
        {
            // Apply creation options if needed
            if ((creationOptions & TaskCreationOptions::RunContinuationsAsynchronously) != TaskCreationOptions::None)
            {
                // Set flag for asynchronous continuations
                m_task->SetRunContinuationsAsynchronously(true);
            }
        }

        explicit TaskCompletionSource(System::Object* state)
            : m_task(std::make_shared<Task<TResult>>())
        {
            m_task->SetAsyncState(state);
        }

        TaskCompletionSource(System::Object* state, TaskCreationOptions creationOptions)
            : m_task(std::make_shared<Task<TResult>>())
        {
            m_task->SetAsyncState(state);
            if ((creationOptions & TaskCreationOptions::RunContinuationsAsynchronously) != TaskCreationOptions::None)
            {
                m_task->SetRunContinuationsAsynchronously(true);
            }
        }

        // Properties
        std::shared_ptr<Task<TResult>> GetTask() const
        {
            return m_task;
        }

        // SetResult methods
        void SetResult(const TResult& result)
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            if (m_completed.load())
            {
                throw System::Exception("The underlying Task is already in one of the three final states: RanToCompletion, Faulted, or Canceled.");
            }
            
            m_task->SetResultInternal(result);
            m_task->SetStatus(TaskStatus::RanToCompletion);
            m_completed.store(true);
            m_task->CompletePromise();
        }

        bool TrySetResult(const TResult& result)
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            if (m_completed.load())
            {
                return false;
            }
            
            try
            {
                m_task->SetResultInternal(result);
                m_task->SetStatus(TaskStatus::RanToCompletion);
                m_completed.store(true);
                m_task->CompletePromise();
                return true;
            }
            catch (...)
            {
                return false;
            }
        }

        // SetException methods
        void SetException(const System::Exception& exception)
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            if (m_completed.load())
            {
                throw System::Exception("The underlying Task is already in one of the three final states: RanToCompletion, Faulted, or Canceled.");
            }
            
            m_task->SetException(std::make_exception_ptr(exception));
            m_task->SetStatus(TaskStatus::Faulted);
            m_completed.store(true);
            m_task->CompletePromiseWithException(std::make_exception_ptr(exception));
        }

        void SetException(const std::vector<System::Exception>& exceptions)
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            if (m_completed.load())
            {
                throw System::Exception("The underlying Task is already in one of the three final states: RanToCompletion, Faulted, or Canceled.");
            }
            
            if (exceptions.empty())
            {
                throw System::Exception("The exceptions collection was empty.");
            }
            
            // For multiple exceptions, we'll use the first one as the primary exception
            auto primaryException = std::make_exception_ptr(exceptions[0]);
            m_task->SetException(primaryException);
            m_task->SetStatus(TaskStatus::Faulted);
            m_completed.store(true);
            m_task->CompletePromiseWithException(primaryException);
        }

        bool TrySetException(const System::Exception& exception)
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            if (m_completed.load())
            {
                return false;
            }
            
            try
            {
                auto exceptionPtr = std::make_exception_ptr(exception);
                m_task->SetException(exceptionPtr);
                m_task->SetStatus(TaskStatus::Faulted);
                m_completed.store(true);
                m_task->CompletePromiseWithException(exceptionPtr);
                return true;
            }
            catch (...)
            {
                return false;
            }
        }

        bool TrySetException(const std::vector<System::Exception>& exceptions)
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            if (m_completed.load() || exceptions.empty())
            {
                return false;
            }
            
            try
            {
                auto primaryException = std::make_exception_ptr(exceptions[0]);
                m_task->SetException(primaryException);
                m_task->SetStatus(TaskStatus::Faulted);
                m_completed.store(true);
                m_task->CompletePromiseWithException(primaryException);
                return true;
            }
            catch (...)
            {
                return false;
            }
        }

        // SetCanceled methods
        void SetCanceled()
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            if (m_completed.load())
            {
                throw System::Exception("The underlying Task is already in one of the three final states: RanToCompletion, Faulted, or Canceled.");
            }
            
            m_task->SetStatus(TaskStatus::Canceled);
            m_completed.store(true);
            m_task->CompletePromiseWithCancellation();
        }

        void SetCanceled(const System::Threading::CancellationToken& cancellationToken)
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            if (m_completed.load())
            {
                throw System::Exception("The underlying Task is already in one of the three final states: RanToCompletion, Faulted, or Canceled.");
            }
            
            m_task->SetCancellationToken(cancellationToken);
            m_task->SetStatus(TaskStatus::Canceled);
            m_completed.store(true);
            m_task->CompletePromiseWithCancellation();
        }

        bool TrySetCanceled()
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            if (m_completed.load())
            {
                return false;
            }
            
            try
            {
                m_task->SetStatus(TaskStatus::Canceled);
                m_completed.store(true);
                m_task->CompletePromiseWithCancellation();
                return true;
            }
            catch (...)
            {
                return false;
            }
        }

        bool TrySetCanceled(const System::Threading::CancellationToken& cancellationToken)
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            if (m_completed.load())
            {
                return false;
            }
            
            try
            {
                m_task->SetCancellationToken(cancellationToken);
                m_task->SetStatus(TaskStatus::Canceled);
                m_completed.store(true);
                m_task->CompletePromiseWithCancellation();
                return true;
            }
            catch (...)
            {
                return false;
            }
        }
    };

    // Specialization for void (non-generic TaskCompletionSource)
    template<>
    class TaskCompletionSource<void> : public System::Object
    {
    private:
        std::shared_ptr<Task> m_task;
        std::atomic<bool> m_completed{false};
        mutable std::mutex m_mutex;

    public:
        // Constructors
        TaskCompletionSource()
            : m_task(std::make_shared<Task>())
        {
        }

        explicit TaskCompletionSource(TaskCreationOptions creationOptions)
            : m_task(std::make_shared<Task>())
        {
            if ((creationOptions & TaskCreationOptions::RunContinuationsAsynchronously) != TaskCreationOptions::None)
            {
                m_task->SetRunContinuationsAsynchronously(true);
            }
        }

        explicit TaskCompletionSource(System::Object* state)
            : m_task(std::make_shared<Task>())
        {
            m_task->SetAsyncState(state);
        }

        TaskCompletionSource(System::Object* state, TaskCreationOptions creationOptions)
            : m_task(std::make_shared<Task>())
        {
            m_task->SetAsyncState(state);
            if ((creationOptions & TaskCreationOptions::RunContinuationsAsynchronously) != TaskCreationOptions::None)
            {
                m_task->SetRunContinuationsAsynchronously(true);
            }
        }

        // Properties
        std::shared_ptr<Task> GetTask() const
        {
            return m_task;
        }

        // SetResult method (void version)
        void SetResult()
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            if (m_completed.load())
            {
                throw System::Exception("The underlying Task is already in one of the three final states: RanToCompletion, Faulted, or Canceled.");
            }
            
            m_task->SetStatus(TaskStatus::RanToCompletion);
            m_completed.store(true);
            m_task->CompletePromise();
        }

        bool TrySetResult()
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            if (m_completed.load())
            {
                return false;
            }
            
            try
            {
                m_task->SetStatus(TaskStatus::RanToCompletion);
                m_completed.store(true);
                m_task->CompletePromise();
                return true;
            }
            catch (...)
            {
                return false;
            }
        }

        // SetException methods (same as generic version)
        void SetException(const System::Exception& exception)
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            if (m_completed.load())
            {
                throw System::Exception("The underlying Task is already in one of the three final states: RanToCompletion, Faulted, or Canceled.");
            }
            
            m_task->SetException(std::make_exception_ptr(exception));
            m_task->SetStatus(TaskStatus::Faulted);
            m_completed.store(true);
            m_task->CompletePromiseWithException(std::make_exception_ptr(exception));
        }

        bool TrySetException(const System::Exception& exception)
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            if (m_completed.load())
            {
                return false;
            }
            
            try
            {
                auto exceptionPtr = std::make_exception_ptr(exception);
                m_task->SetException(exceptionPtr);
                m_task->SetStatus(TaskStatus::Faulted);
                m_completed.store(true);
                m_task->CompletePromiseWithException(exceptionPtr);
                return true;
            }
            catch (...)
            {
                return false;
            }
        }

        // SetCanceled methods (same as generic version)
        void SetCanceled()
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            if (m_completed.load())
            {
                throw System::Exception("The underlying Task is already in one of the three final states: RanToCompletion, Faulted, or Canceled.");
            }
            
            m_task->SetStatus(TaskStatus::Canceled);
            m_completed.store(true);
            m_task->CompletePromiseWithCancellation();
        }

        bool TrySetCanceled()
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            if (m_completed.load())
            {
                return false;
            }
            
            try
            {
                m_task->SetStatus(TaskStatus::Canceled);
                m_completed.store(true);
                m_task->CompletePromiseWithCancellation();
                return true;
            }
            catch (...)
            {
                return false;
            }
        }
    };
}
