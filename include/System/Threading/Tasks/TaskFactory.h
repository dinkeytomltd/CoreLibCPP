
#pragma once

#include "System/Object.h"
#include "Task.h"
#include "TaskCreationOptions.h"
#include "TaskContinuationOptions.h"
#include "TaskScheduler.h"
#include "System/Threading/CancellationToken.h"
#include <functional>
#include <memory>

namespace System::Threading::Tasks
{
    class TaskFactory : public System::Object
    {
    private:
        std::shared_ptr<TaskScheduler> m_scheduler;
        TaskCreationOptions m_creationOptions;
        TaskContinuationOptions m_continuationOptions;
        System::Threading::CancellationToken m_cancellationToken;

    public:
        // Constructors
        TaskFactory();
        explicit TaskFactory(const System::Threading::CancellationToken& cancellationToken);
        explicit TaskFactory(std::shared_ptr<TaskScheduler> scheduler);
        explicit TaskFactory(TaskCreationOptions creationOptions, TaskContinuationOptions continuationOptions);
        TaskFactory(const System::Threading::CancellationToken& cancellationToken,
                   TaskCreationOptions creationOptions,
                   TaskContinuationOptions continuationOptions,
                   std::shared_ptr<TaskScheduler> scheduler);

        // Properties
        System::Threading::CancellationToken GetCancellationToken() const;
        TaskContinuationOptions GetContinuationOptions() const;
        TaskCreationOptions GetCreationOptions() const;
        std::shared_ptr<TaskScheduler> GetScheduler() const;

        // StartNew overloads for Action delegates
        std::shared_ptr<Task> StartNew(std::function<void()> action);
        std::shared_ptr<Task> StartNew(std::function<void()> action, const System::Threading::CancellationToken& cancellationToken);
        std::shared_ptr<Task> StartNew(std::function<void()> action, TaskCreationOptions creationOptions);
        std::shared_ptr<Task> StartNew(std::function<void()> action,
                                      const System::Threading::CancellationToken& cancellationToken,
                                      TaskCreationOptions creationOptions,
                                      std::shared_ptr<TaskScheduler> scheduler);

        // StartNew overloads for Action<Object> delegates with state
        std::shared_ptr<Task> StartNew(std::function<void(System::Object*)> action, System::Object* state);
        std::shared_ptr<Task> StartNew(std::function<void(System::Object*)> action, System::Object* state,
                                      const System::Threading::CancellationToken& cancellationToken);
        std::shared_ptr<Task> StartNew(std::function<void(System::Object*)> action, System::Object* state,
                                      TaskCreationOptions creationOptions);
        std::shared_ptr<Task> StartNew(std::function<void(System::Object*)> action, System::Object* state,
                                      const System::Threading::CancellationToken& cancellationToken,
                                      TaskCreationOptions creationOptions,
                                      std::shared_ptr<TaskScheduler> scheduler);

        // StartNew overloads for Func<TResult> delegates
        template<typename TResult>
        std::shared_ptr<Task<TResult>> StartNew(std::function<TResult()> function)
        {
            return StartNewInternal<TResult>(function, nullptr, m_cancellationToken, m_creationOptions, m_scheduler);
        }

        template<typename TResult>
        std::shared_ptr<Task<TResult>> StartNew(std::function<TResult()> function,
                                               const System::Threading::CancellationToken& cancellationToken)
        {
            return StartNewInternal<TResult>(function, nullptr, cancellationToken, m_creationOptions, m_scheduler);
        }

        template<typename TResult>
        std::shared_ptr<Task<TResult>> StartNew(std::function<TResult()> function,
                                               TaskCreationOptions creationOptions)
        {
            return StartNewInternal<TResult>(function, nullptr, m_cancellationToken, creationOptions, m_scheduler);
        }

        template<typename TResult>
        std::shared_ptr<Task<TResult>> StartNew(std::function<TResult()> function,
                                               const System::Threading::CancellationToken& cancellationToken,
                                               TaskCreationOptions creationOptions,
                                               std::shared_ptr<TaskScheduler> scheduler)
        {
            return StartNewInternal<TResult>(function, nullptr, cancellationToken, creationOptions, scheduler);
        }

        // StartNew overloads for Func<Object, TResult> delegates with state
        template<typename TResult>
        std::shared_ptr<Task<TResult>> StartNew(std::function<TResult(System::Object*)> function, System::Object* state)
        {
            return StartNewInternal<TResult>(function, state, m_cancellationToken, m_creationOptions, m_scheduler);
        }

        template<typename TResult>
        std::shared_ptr<Task<TResult>> StartNew(std::function<TResult(System::Object*)> function, System::Object* state,
                                               const System::Threading::CancellationToken& cancellationToken)
        {
            return StartNewInternal<TResult>(function, state, cancellationToken, m_creationOptions, m_scheduler);
        }

        template<typename TResult>
        std::shared_ptr<Task<TResult>> StartNew(std::function<TResult(System::Object*)> function, System::Object* state,
                                               TaskCreationOptions creationOptions)
        {
            return StartNewInternal<TResult>(function, state, m_cancellationToken, creationOptions, m_scheduler);
        }

        template<typename TResult>
        std::shared_ptr<Task<TResult>> StartNew(std::function<TResult(System::Object*)> function, System::Object* state,
                                               const System::Threading::CancellationToken& cancellationToken,
                                               TaskCreationOptions creationOptions,
                                               std::shared_ptr<TaskScheduler> scheduler)
        {
            return StartNewInternal<TResult>(function, state, cancellationToken, creationOptions, scheduler);
        }

        // ContinueWhenAll overloads
        std::shared_ptr<Task> ContinueWhenAll(const std::vector<std::shared_ptr<Task>>& tasks,
                                             std::function<void(const std::vector<std::shared_ptr<Task>>&)> continuationAction);
        
        template<typename TResult>
        std::shared_ptr<Task<TResult>> ContinueWhenAll(const std::vector<std::shared_ptr<Task>>& tasks,
                                                      std::function<TResult(const std::vector<std::shared_ptr<Task>>&)> continuationFunction)
        {
            return ContinueWhenAllInternal<TResult>(tasks, continuationFunction, m_cancellationToken, m_continuationOptions, m_scheduler);
        }

        // ContinueWhenAny overloads
        std::shared_ptr<Task> ContinueWhenAny(const std::vector<std::shared_ptr<Task>>& tasks,
                                             std::function<void(std::shared_ptr<Task>)> continuationAction);
        
        template<typename TResult>
        std::shared_ptr<Task<TResult>> ContinueWhenAny(const std::vector<std::shared_ptr<Task>>& tasks,
                                                      std::function<TResult(std::shared_ptr<Task>)> continuationFunction)
        {
            return ContinueWhenAnyInternal<TResult>(tasks, continuationFunction, m_cancellationToken, m_continuationOptions, m_scheduler);
        }

        // FromAsync overloads (simplified implementation)
        std::shared_ptr<Task> FromAsync(std::function<void(std::function<void()>)> beginMethod,
                                       std::function<void()> endMethod,
                                       System::Object* state);

        template<typename TResult>
        std::shared_ptr<Task<TResult>> FromAsync(std::function<void(std::function<void()>)> beginMethod,
                                                std::function<TResult()> endMethod,
                                                System::Object* state)
        {
            return FromAsyncInternal<TResult>(beginMethod, endMethod, state);
        }

    private:
        // Internal helper methods
        std::shared_ptr<Task> StartNewInternal(std::function<void()> action,
                                              const System::Threading::CancellationToken& cancellationToken,
                                              TaskCreationOptions creationOptions,
                                              std::shared_ptr<TaskScheduler> scheduler);

        std::shared_ptr<Task> StartNewInternal(std::function<void(System::Object*)> action, System::Object* state,
                                              const System::Threading::CancellationToken& cancellationToken,
                                              TaskCreationOptions creationOptions,
                                              std::shared_ptr<TaskScheduler> scheduler);

        template<typename TResult>
        std::shared_ptr<Task<TResult>> StartNewInternal(std::function<TResult()> function, System::Object* state,
                                                       const System::Threading::CancellationToken& cancellationToken,
                                                       TaskCreationOptions creationOptions,
                                                       std::shared_ptr<TaskScheduler> scheduler);

        template<typename TResult>
        std::shared_ptr<Task<TResult>> StartNewInternal(std::function<TResult(System::Object*)> function, System::Object* state,
                                                       const System::Threading::CancellationToken& cancellationToken,
                                                       TaskCreationOptions creationOptions,
                                                       std::shared_ptr<TaskScheduler> scheduler);

        template<typename TResult>
        std::shared_ptr<Task<TResult>> ContinueWhenAllInternal(const std::vector<std::shared_ptr<Task>>& tasks,
                                                              std::function<TResult(const std::vector<std::shared_ptr<Task>>&)> continuationFunction,
                                                              const System::Threading::CancellationToken& cancellationToken,
                                                              TaskContinuationOptions continuationOptions,
                                                              std::shared_ptr<TaskScheduler> scheduler);

        template<typename TResult>
        std::shared_ptr<Task<TResult>> ContinueWhenAnyInternal(const std::vector<std::shared_ptr<Task>>& tasks,
                                                              std::function<TResult(std::shared_ptr<Task>)> continuationFunction,
                                                              const System::Threading::CancellationToken& cancellationToken,
                                                              TaskContinuationOptions continuationOptions,
                                                              std::shared_ptr<TaskScheduler> scheduler);

        template<typename TResult>
        std::shared_ptr<Task<TResult>> FromAsyncInternal(std::function<void(std::function<void()>)> beginMethod,
                                                        std::function<TResult()> endMethod,
                                                        System::Object* state);

        void ValidateCreationOptions(TaskCreationOptions creationOptions);
        void ValidateContinuationOptions(TaskContinuationOptions continuationOptions);
    };

    // Generic TaskFactory<TResult>
    template<typename TResult>
    class TaskFactory : public System::Object
    {
    private:
        std::shared_ptr<TaskScheduler> m_scheduler;
        TaskCreationOptions m_creationOptions;
        TaskContinuationOptions m_continuationOptions;
        System::Threading::CancellationToken m_cancellationToken;

    public:
        // Constructors
        TaskFactory();
        explicit TaskFactory(const System::Threading::CancellationToken& cancellationToken);
        explicit TaskFactory(std::shared_ptr<TaskScheduler> scheduler);
        explicit TaskFactory(TaskCreationOptions creationOptions, TaskContinuationOptions continuationOptions);
        TaskFactory(const System::Threading::CancellationToken& cancellationToken,
                   TaskCreationOptions creationOptions,
                   TaskContinuationOptions continuationOptions,
                   std::shared_ptr<TaskScheduler> scheduler);

        // Properties
        System::Threading::CancellationToken GetCancellationToken() const;
        TaskContinuationOptions GetContinuationOptions() const;
        TaskCreationOptions GetCreationOptions() const;
        std::shared_ptr<TaskScheduler> GetScheduler() const;

        // StartNew overloads for Func<TResult> delegates
        std::shared_ptr<Task<TResult>> StartNew(std::function<TResult()> function);
        std::shared_ptr<Task<TResult>> StartNew(std::function<TResult()> function,
                                               const System::Threading::CancellationToken& cancellationToken);
        std::shared_ptr<Task<TResult>> StartNew(std::function<TResult()> function,
                                               TaskCreationOptions creationOptions);
        std::shared_ptr<Task<TResult>> StartNew(std::function<TResult()> function,
                                               const System::Threading::CancellationToken& cancellationToken,
                                               TaskCreationOptions creationOptions,
                                               std::shared_ptr<TaskScheduler> scheduler);

        // StartNew overloads for Func<Object, TResult> delegates with state
        std::shared_ptr<Task<TResult>> StartNew(std::function<TResult(System::Object*)> function, System::Object* state);
        std::shared_ptr<Task<TResult>> StartNew(std::function<TResult(System::Object*)> function, System::Object* state,
                                               const System::Threading::CancellationToken& cancellationToken);
        std::shared_ptr<Task<TResult>> StartNew(std::function<TResult(System::Object*)> function, System::Object* state,
                                               TaskCreationOptions creationOptions);
        std::shared_ptr<Task<TResult>> StartNew(std::function<TResult(System::Object*)> function, System::Object* state,
                                               const System::Threading::CancellationToken& cancellationToken,
                                               TaskCreationOptions creationOptions,
                                               std::shared_ptr<TaskScheduler> scheduler);

        // ContinueWhenAll and ContinueWhenAny overloads
        std::shared_ptr<Task<TResult>> ContinueWhenAll(const std::vector<std::shared_ptr<Task>>& tasks,
                                                      std::function<TResult(const std::vector<std::shared_ptr<Task>>&)> continuationFunction);
        
        std::shared_ptr<Task<TResult>> ContinueWhenAny(const std::vector<std::shared_ptr<Task>>& tasks,
                                                      std::function<TResult(std::shared_ptr<Task>)> continuationFunction);

        // FromAsync overloads
        std::shared_ptr<Task<TResult>> FromAsync(std::function<void(std::function<void()>)> beginMethod,
                                                std::function<TResult()> endMethod,
                                                System::Object* state);

    private:
        // Internal helper methods
        std::shared_ptr<Task<TResult>> StartNewInternal(std::function<TResult()> function, System::Object* state,
                                                       const System::Threading::CancellationToken& cancellationToken,
                                                       TaskCreationOptions creationOptions,
                                                       std::shared_ptr<TaskScheduler> scheduler);

        std::shared_ptr<Task<TResult>> StartNewInternal(std::function<TResult(System::Object*)> function, System::Object* state,
                                                       const System::Threading::CancellationToken& cancellationToken,
                                                       TaskCreationOptions creationOptions,
                                                       std::shared_ptr<TaskScheduler> scheduler);
    };
}
