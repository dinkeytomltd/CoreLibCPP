
#pragma once

#include "WaitHandle.h"

namespace System::Threading
{
    enum class EventResetMode
    {
        AutoReset = 0,
        ManualReset = 1
    };

    class EventWaitHandle : public WaitHandle
    {
    protected:
        EventResetMode resetMode;
        std::atomic<bool> isSet;

    public:
        EventWaitHandle(bool initialState, EventResetMode mode);
        virtual ~EventWaitHandle() = default;

        virtual bool Set();
        virtual bool Reset();

        // WaitHandle overrides
        bool WaitOne() override;
        bool WaitOne(int millisecondsTimeout) override;
        bool WaitOne(const TimeSpan& timeout) override;
    };
}
