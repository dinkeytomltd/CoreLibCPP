
#pragma once

#include "WaitHandle.h"
#include "System/String.h"
#include <mutex>
#include <memory>

namespace System::Threading
{
    class Mutex : public WaitHandle
    {
    private:
        std::unique_ptr<std::recursive_mutex> nativeMutex;
        System::String name;
        bool initiallyOwned;
        std::atomic<bool> owned;

    public:
        Mutex();
        Mutex(bool initiallyOwned);
        Mutex(bool initiallyOwned, const System::String& name);
        virtual ~Mutex();

        void ReleaseMutex();

        // WaitHandle overrides
        bool WaitOne() override;
        bool WaitOne(int millisecondsTimeout) override;
        bool WaitOne(const TimeSpan& timeout) override;
        void Close() override;

        // Static methods
        static Mutex* OpenExisting(const System::String& name);
        static bool TryOpenExisting(const System::String& name, Mutex*& result);
    };
}
