
#pragma once

#include "System/Object.h"
#include "Task.h"
#include <memory>

namespace System::Threading::Tasks
{
    class TaskScheduler : public System::Object
    {
    public:
        virtual ~TaskScheduler() = default;
        virtual void QueueTask(std::shared_ptr<Task> task) = 0;
        virtual bool TryExecuteTaskInline(std::shared_ptr<Task> task, bool taskWasPreviouslyQueued) = 0;

        static TaskScheduler* GetDefault();
        static TaskScheduler* GetCurrent();
    };

    class ThreadPoolTaskScheduler : public TaskScheduler
    {
    public:
        void QueueTask(std::shared_ptr<Task> task) override;
        bool TryExecuteTaskInline(std::shared_ptr<Task> task, bool taskWasPreviouslyQueued) override;
    };
}
