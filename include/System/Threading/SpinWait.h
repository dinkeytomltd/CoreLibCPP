
#pragma once

#include "System/TimeSpan.h"
#include <functional>
#include <atomic>

namespace System::Threading
{
    struct SpinWait
    {
    private:
        int count;

    public:
        SpinWait();

        void SpinOnce();
        void Reset();

        // Properties
        int GetCount() const;
        bool GetNextSpinWillYield() const;

        // Static methods
        static void SpinUntil(std::function<bool()> condition);
        static bool SpinUntil(std::function<bool()> condition, int millisecondsTimeout);
        static bool SpinUntil(std::function<bool()> condition, const TimeSpan& timeout);
    };
}
