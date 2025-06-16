
#pragma once

#include "System/Object.h"
#include "System/TimeSpan.h"
#include <unordered_map>
#include <mutex>
#include <condition_variable>
#include <memory>

namespace System::Threading
{
    class Monitor
    {
    public:
        static void Enter(System::Object* obj);
        static void Enter(System::Object* obj, bool& lockTaken);
        static bool TryEnter(System::Object* obj);
        static bool TryEnter(System::Object* obj, int millisecondsTimeout);
        static bool TryEnter(System::Object* obj, const TimeSpan& timeout);
        static void Exit(System::Object* obj);

        static void Wait(System::Object* obj);
        static bool Wait(System::Object* obj, int millisecondsTimeout);
        static bool Wait(System::Object* obj, const TimeSpan& timeout);
        static void Pulse(System::Object* obj);
        static void PulseAll(System::Object* obj);

    private:
        struct MonitorData
        {
            std::recursive_mutex mutex;
            std::condition_variable_any condition;
            std::atomic<int> waitingCount{0};
        };

        static std::unordered_map<System::Object*, std::unique_ptr<MonitorData>> monitors;
        static std::mutex globalMutex;

        static MonitorData* GetOrCreateMonitor(System::Object* obj);
    };
}
