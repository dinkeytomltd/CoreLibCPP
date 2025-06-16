
#pragma once

#include "System/Object.h"
#include "System/String.h"
#include "System/TimeSpan.h"
#include <thread>
#include <functional>
#include <atomic>
#include <memory>

namespace System::Threading
{
    enum class ThreadState
    {
        Unstarted = 8,
        Running = 0,
        StopRequested = 1,
        SuspendRequested = 2,
        Background = 4,
        Stopped = 16,
        WaitSleepJoin = 32,
        Suspended = 64,
        AbortRequested = 128,
        Aborted = 256
    };

    enum class ThreadPriority
    {
        Lowest = 0,
        BelowNormal = 1,
        Normal = 2,
        AboveNormal = 3,
        Highest = 4
    };

    using ThreadStart = std::function<void()>;
    using ParameterizedThreadStart = std::function<void(System::Object*)>;

    class Thread : public System::Object
    {
    private:
        std::unique_ptr<std::jthread> nativeThread;
        ThreadStart threadStart;
        ParameterizedThreadStart parameterizedThreadStart;
        System::Object* parameter;
        std::atomic<ThreadState> state;
        ThreadPriority priority;
        System::String name;
        std::atomic<bool> isBackground;
        static thread_local Thread* currentThread;
        static std::atomic<int> nextThreadId;
        int managedThreadId;

    public:
        Thread(ThreadStart start);
        Thread(ParameterizedThreadStart start);
        virtual ~Thread();

        void Start();
        void Start(System::Object* parameter);
        void Join();
        bool Join(int millisecondsTimeout);
        void Abort();
        void Interrupt();

        // Properties
        ThreadState GetThreadState() const;
        ThreadPriority GetPriority() const;
        void SetPriority(ThreadPriority value);
        System::String GetName() const;
        void SetName(const System::String& value);
        bool GetIsBackground() const;
        void SetIsBackground(bool value);
        bool GetIsAlive() const;
        int GetManagedThreadId() const;

        // Static methods
        static Thread* GetCurrentThread();
        static void Sleep(int millisecondsTimeout);
        static void Sleep(const TimeSpan& timeout);
        static void Yield();
        static void SpinWait(int iterations);

    private:
        void ThreadProc();
        void ParameterizedThreadProc();
    };
}
