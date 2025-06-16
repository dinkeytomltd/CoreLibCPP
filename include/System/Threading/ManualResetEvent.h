
#pragma once

#include "EventWaitHandle.h"

namespace System::Threading
{
    class ManualResetEvent : public EventWaitHandle
    {
    public:
        ManualResetEvent(bool initialState);
        virtual ~ManualResetEvent() = default;

        bool Set() override;
        bool Reset() override;
    };
}
