
#pragma once

#include "System/Object.h"
#include "System/TimeSpan.h"
#include "WaitHandle.h"
#include <functional>
#include <thread>
#include <atomic>
#include <mutex>
#include <condition_variable>

namespace System::Threading
{
    using TimerCallback = std::function<void(System::Object*)>;

    class Timer : public System::Object
    {
    private:
        TimerCallback callback;
        System::Object* state;
        std::unique_ptr<std::jthread> timerThread;
        std::atomic<bool> disposed{false};
        std::mutex mutex;
        std::condition_variable condition;
        std::atomic<int> dueTime;
        std::atomic<int> period;

    public:
        Timer(TimerCallback callback, System::Object* state, int dueTime, int period);
        Timer(TimerCallback callback, System::Object* state, const TimeSpan& dueTime, const TimeSpan& period);
        Timer(TimerCallback callback, System::Object* state, long dueTime, long period);
        virtual ~Timer();

        bool Change(int dueTime, int period);
        bool Change(const TimeSpan& dueTime, const TimeSpan& period);
        bool Change(long dueTime, long period);

        void Dispose();
        bool Dispose(WaitHandle* notifyObject);

    private:
        void TimerThreadProc(std::stop_token stopToken);
    };
}
