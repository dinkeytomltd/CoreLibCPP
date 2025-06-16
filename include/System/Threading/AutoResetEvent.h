
#pragma once

#include "EventWaitHandle.h"

namespace System::Threading
{
    class AutoResetEvent : public EventWaitHandle
    {
    public:
        AutoResetEvent(bool initialState);
        virtual ~AutoResetEvent() = default;

        bool Set() override;
        bool Reset() override;
    };
}
