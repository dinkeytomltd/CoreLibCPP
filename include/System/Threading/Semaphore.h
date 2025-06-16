
#pragma once

#include "WaitHandle.h"
#include "System/String.h"
#include <semaphore>
#include <memory>

namespace System::Threading
{
    class Semaphore : public WaitHandle
    {
    private:
        std::unique_ptr<std::counting_semaphore<>> nativeSemaphore;
        std::atomic<int> currentCount;
        int maximumCount;
        System::String name;

    public:
        Semaphore(int initialCount, int maximumCount);
        Semaphore(int initialCount, int maximumCount, const System::String& name);
        virtual ~Semaphore();

        int Release();
        int Release(int releaseCount);

        // WaitHandle overrides
        bool WaitOne() override;
        bool WaitOne(int millisecondsTimeout) override;
        bool WaitOne(const TimeSpan& timeout) override;

        // Static methods
        static Semaphore* OpenExisting(const System::String& name);
        static bool TryOpenExisting(const System::String& name, Semaphore*& result);

        // Properties
        int GetCurrentCount() const;
    };
}
