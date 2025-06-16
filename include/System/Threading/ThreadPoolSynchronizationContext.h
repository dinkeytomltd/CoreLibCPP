
#pragma once

#include "System/Threading/SynchronizationContext.h"

namespace System {
namespace Threading {

class ThreadPoolSynchronizationContext : public SynchronizationContext {
public:
    ThreadPoolSynchronizationContext();
    ~ThreadPoolSynchronizationContext() override;

    // Override core methods
    void Post(std::function<void(void*)> callback, void* state) override;
    void Send(std::function<void(void*)> callback, void* state) override;
    std::shared_ptr<SynchronizationContext> CreateCopy() override;

    // ThreadPool-specific methods
    static std::shared_ptr<ThreadPoolSynchronizationContext> Instance();

private:
    static std::shared_ptr<ThreadPoolSynchronizationContext> s_instance;
    static std::once_flag s_instanceFlag;
};

} // namespace Threading
} // namespace System
