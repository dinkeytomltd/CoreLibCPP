
#pragma once

#include "System/TimeSpan.h"
#include <atomic>

namespace System::Threading
{
    struct SpinLock
    {
    private:
        std::atomic_flag flag = ATOMIC_FLAG_INIT;
        bool isThreadOwnerTrackingEnabled;
        thread_local static bool isHeldByCurrentThread;

    public:
        SpinLock(bool enableThreadOwnerTracking);

        void Enter(bool& lockTaken);
        bool TryEnter(bool& lockTaken);
        bool TryEnter(int millisecondsTimeout, bool& lockTaken);
        bool TryEnter(const TimeSpan& timeout, bool& lockTaken);
        void Exit();
        void Exit(bool useMemoryBarrier);

        // Properties
        bool GetIsHeld() const;
        bool GetIsHeldByCurrentThread() const;
        bool GetIsThreadOwnerTrackingEnabled() const;
    };
}
