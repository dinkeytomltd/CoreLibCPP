
#pragma once

#include "System/Object.h"
#include <functional>
#include <memory>
#include <thread>
#include <mutex>
#include <atomic>
#include <exception>
#include <vector>
#include <condition_variable>

namespace System {
namespace Threading {

// Forward declarations
class SendOrPostCallback;

class SynchronizationContext : public System::Object {
public:
    // Constructor and destructor
    SynchronizationContext();
    virtual ~SynchronizationContext();

    // Core virtual methods
    virtual void Post(std::function<void(void*)> callback, void* state);
    virtual void Send(std::function<void(void*)> callback, void* state);
    virtual std::shared_ptr<SynchronizationContext> CreateCopy();

    // Operation tracking
    virtual void OperationStarted();
    virtual void OperationCompleted();

    // Static context management
    static std::shared_ptr<SynchronizationContext> Current();
    static void SetSynchronizationContext(std::shared_ptr<SynchronizationContext> context);

    // Utility methods
    virtual bool IsWaitNotificationRequired() const;
    virtual void Wait(const std::vector<std::shared_ptr<void>>& waitHandles, bool waitAll, int millisecondsTimeout);

protected:
    // Protected helper methods
    void SetWaitNotificationRequired();

private:
    // Thread-local storage for current context
    static thread_local std::shared_ptr<SynchronizationContext> s_current;
    
    // Operation tracking
    std::atomic<int> m_operationCount{0};
    bool m_waitNotificationRequired{false};
    mutable std::mutex m_mutex;
};

// Callback delegate type
using SendOrPostCallback = std::function<void(void*)>;

} // namespace Threading
} // namespace System
