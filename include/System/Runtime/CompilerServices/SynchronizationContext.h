
#pragma once

#include "System/Object.h"
#include <functional>

namespace System::Runtime::CompilerServices
{
    class SynchronizationContext : public System::Object
    {
    public:
        virtual ~SynchronizationContext() = default;
        virtual void Post(std::function<void(System::Object*)> d, System::Object* state);
        virtual void Send(std::function<void(System::Object*)> d, System::Object* state);

        // Properties
        static SynchronizationContext* GetCurrent();
        static void SetSynchronizationContext(SynchronizationContext* syncContext);

    protected:
        virtual SynchronizationContext* CreateCopy();

    private:
        static thread_local SynchronizationContext* current;
    };
}
