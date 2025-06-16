
#pragma once

#include "System/Object.h"
#include "System/TimeSpan.h"
#include "CancellationToken.h"
#include "WaitHandle.h"
#include <atomic>
#include <mutex>
#include <condition_variable>

namespace System::Threading
{
    class ManualResetEventSlim : public System::Object
    {
    private:
        std::atomic<bool> isSet;
        std::mutex mutex;
        std::condition_variable condition;
        int spinCount;
        std::unique_ptr<WaitHandle> waitHandle;

    public:
        ManualResetEventSlim();
        ManualResetEventSlim(bool initialState);
        ManualResetEventSlim(bool initialState, int spinCount);
        virtual ~ManualResetEventSlim() = default;

        void Set();
        void Reset();
        void Wait();
        bool Wait(int millisecondsTimeout);
        bool Wait(const TimeSpan& timeout);
        bool Wait(const CancellationToken& cancellationToken);
        bool Wait(int millisecondsTimeout, const CancellationToken& cancellationToken);

        // Properties
        bool GetIsSet() const;
        int GetSpinCount() const;
        WaitHandle* GetWaitHandle();
    };
}
