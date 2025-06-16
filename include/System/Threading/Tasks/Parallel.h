
#pragma once

#include "System/Object.h"
#include "../CancellationToken.h"
#include "TaskScheduler.h"
#include <functional>
#include <vector>

namespace System::Threading::Tasks
{
    struct ParallelLoopResult
    {
        bool IsCompleted;
        long LowestBreakIteration;

        ParallelLoopResult(bool isCompleted, long lowestBreakIteration);
    };

    class ParallelLoopState
    {
    private:
        std::atomic<bool> isStopped{false};
        std::atomic<bool> isExceptional{false};
        std::atomic<long> lowestBreakIteration{LONG_MAX};

    public:
        void Stop();
        void Break();

        // Properties
        bool GetIsStopped() const;
        bool GetIsExceptional() const;
        long GetLowestBreakIteration() const;
        bool GetShouldExitCurrentIteration() const;
    };

    class ParallelOptions
    {
    private:
        CancellationToken cancellationToken;
        int maxDegreeOfParallelism;
        TaskScheduler* taskScheduler;

    public:
        ParallelOptions();

        // Properties
        CancellationToken GetCancellationToken() const;
        void SetCancellationToken(const CancellationToken& value);

        int GetMaxDegreeOfParallelism() const;
        void SetMaxDegreeOfParallelism(int value);

        TaskScheduler* GetTaskScheduler() const;
        void SetTaskScheduler(TaskScheduler* value);
    };

    class Parallel
    {
    public:
        // Parallel.For
        static ParallelLoopResult For(int fromInclusive, int toExclusive, std::function<void(int)> body);
        static ParallelLoopResult For(int fromInclusive, int toExclusive, const ParallelOptions& parallelOptions, std::function<void(int)> body);
        static ParallelLoopResult For(int fromInclusive, int toExclusive, std::function<void(int, ParallelLoopState&)> body);
        static ParallelLoopResult For(int fromInclusive, int toExclusive, const ParallelOptions& parallelOptions, std::function<void(int, ParallelLoopState&)> body);

        static ParallelLoopResult For(long fromInclusive, long toExclusive, std::function<void(long)> body);
        static ParallelLoopResult For(long fromInclusive, long toExclusive, const ParallelOptions& parallelOptions, std::function<void(long)> body);

        // Parallel.ForEach
        template<typename T>
        static ParallelLoopResult ForEach(const std::vector<T>& source, std::function<void(T)> body);

        template<typename T>
        static ParallelLoopResult ForEach(const std::vector<T>& source, const ParallelOptions& parallelOptions, std::function<void(T)> body);

        template<typename T>
        static ParallelLoopResult ForEach(const std::vector<T>& source, std::function<void(T, ParallelLoopState&)> body);

        template<typename T>
        static ParallelLoopResult ForEach(const std::vector<T>& source, const ParallelOptions& parallelOptions, std::function<void(T, ParallelLoopState&)> body);

        // Parallel.Invoke
        static void Invoke(const std::vector<std::function<void()>>& actions);
        static void Invoke(const ParallelOptions& parallelOptions, const std::vector<std::function<void()>>& actions);
    };
}
