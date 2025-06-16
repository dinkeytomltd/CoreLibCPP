
#include "System/Runtime/CompilerServices/SynchronizationContext.h"
#include "System/Threading/ThreadPool.h"

namespace System::Runtime::CompilerServices
{
    thread_local SynchronizationContext* SynchronizationContext::current = nullptr;

    void SynchronizationContext::Post(std::function<void(System::Object*)> d, System::Object* state)
    {
        // Default implementation uses ThreadPool
        System::Threading::ThreadPool::QueueUserWorkItem([d, state](System::Object*) {
            d(state);
        });
    }

    void SynchronizationContext::Send(std::function<void(System::Object*)> d, System::Object* state)
    {
        // Default implementation executes synchronously
        d(state);
    }

    SynchronizationContext* SynchronizationContext::GetCurrent()
    {
        return current;
    }

    void SynchronizationContext::SetSynchronizationContext(SynchronizationContext* syncContext)
    {
        current = syncContext;
    }

    SynchronizationContext* SynchronizationContext::CreateCopy()
    {
        return new SynchronizationContext(*this);
    }
}
