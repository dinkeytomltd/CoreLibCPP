
#pragma once

#include "System/Object.h"
#include <functional>
#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>

namespace System::Threading
{
    using WaitCallback = std::function<void(System::Object*)>;

    class ThreadPool
    {
    public:
        static bool QueueUserWorkItem(WaitCallback callback);
        static bool QueueUserWorkItem(WaitCallback callback, System::Object* state);
        static void GetMaxThreads(int& workerThreads, int& completionPortThreads);
        static void GetMinThreads(int& workerThreads, int& completionPortThreads);
        static bool SetMaxThreads(int workerThreads, int completionPortThreads);
        static bool SetMinThreads(int workerThreads, int completionPortThreads);
        static void GetAvailableThreads(int& workerThreads, int& completionPortThreads);

    private:
        struct WorkItem
        {
            WaitCallback callback;
            System::Object* state;
        };

        static std::vector<std::jthread> workerThreads;
        static std::queue<WorkItem> workQueue;
        static std::mutex queueMutex;
        static std::condition_variable condition;
        static std::atomic<bool> shutdown;
        static std::atomic<int> maxWorkerThreads;
        static std::atomic<int> minWorkerThreads;
        static std::atomic<int> maxCompletionPortThreads;
        static std::atomic<int> minCompletionPortThreads;
        static std::atomic<int> activeThreads;

        static void WorkerThreadProc(std::stop_token stopToken);
        static void EnsureMinimumThreads();
        static void Initialize();
        static std::once_flag initFlag;
    };
}
