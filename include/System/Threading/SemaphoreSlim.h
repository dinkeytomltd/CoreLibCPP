
#pragma once

#include "System/Object.h"
#include "System/TimeSpan.h"
#include <semaphore>
#include <memory>
#include <atomic>

namespace System::Threading
{
    struct CancellationToken;
    class WaitHandle;

    namespace Tasks
    {
        template<typename T> class Task;
    }

    class SemaphoreSlim : public System::Object
    {
    private:
        std::unique_ptr<std::counting_semaphore<>> semaphore;
        std::atomic<int> currentCount;
        int maximumCount;

    public:
        SemaphoreSlim(int initialCount);
        SemaphoreSlim(int initialCount, int maxCount);
        virtual ~SemaphoreSlim();

        void Wait();
        bool Wait(int millisecondsTimeout);
        bool Wait(const TimeSpan& timeout);
        bool Wait(const CancellationToken& cancellationToken);
        bool Wait(int millisecondsTimeout, const CancellationToken& cancellationToken);

        Tasks::Task<void> WaitAsync();
        Tasks::Task<bool> WaitAsync(int millisecondsTimeout);
        Tasks::Task<bool> WaitAsync(const TimeSpan& timeout);
        Tasks::Task<bool> WaitAsync(const CancellationToken& cancellationToken);

        int Release();
        int Release(int releaseCount);

        // Properties
        int GetCurrentCount() const;
        WaitHandle* GetAvailableWaitHandle();
    };
}
