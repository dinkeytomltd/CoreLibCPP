
#pragma once

#include "System/Object.h"
#include "System/TimeSpan.h"
#include <mutex>
#include <condition_variable>
#include <atomic>

namespace System::Threading
{
    class WaitHandle : public System::Object
    {
    protected:
        std::mutex mutex;
        std::condition_variable condition;
        std::atomic<bool> signaled;

    public:
        WaitHandle();
        virtual ~WaitHandle();

        virtual bool WaitOne();
        virtual bool WaitOne(int millisecondsTimeout);
        virtual bool WaitOne(const TimeSpan& timeout);
        virtual void Close();

        static bool WaitAll(const std::vector<WaitHandle*>& waitHandles);
        static bool WaitAll(const std::vector<WaitHandle*>& waitHandles, int millisecondsTimeout);
        static int WaitAny(const std::vector<WaitHandle*>& waitHandles);
        static int WaitAny(const std::vector<WaitHandle*>& waitHandles, int millisecondsTimeout);

    protected:
        virtual void SetSignaled(bool value);
    };
}
