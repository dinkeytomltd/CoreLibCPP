
#pragma once

#include "System/Object.h"
#include "WaitHandle.h"
#include <functional>
#include <memory>
#include <atomic>
#include <vector>

namespace System::Threading
{
    class CancellationTokenRegistration;
    class CancellationTokenSource;

    struct CancellationTokenState
    {
        std::atomic<bool> isCancellationRequested{false};
        std::vector<std::function<void()>> callbacks;
        std::mutex callbacksMutex;
        std::unique_ptr<WaitHandle> waitHandle;
    };

    struct CancellationToken
    {
    private:
        std::shared_ptr<CancellationTokenState> state;

    public:
        CancellationToken();
        CancellationToken(bool canceled);
        CancellationToken(std::shared_ptr<CancellationTokenState> tokenState);

        void ThrowIfCancellationRequested() const;

        // Properties
        bool GetIsCancellationRequested() const;
        bool GetCanBeCanceled() const;
        WaitHandle* GetWaitHandle();

        // Registration
        CancellationTokenRegistration Register(std::function<void()> callback);
        CancellationTokenRegistration Register(std::function<void(System::Object*)> callback, System::Object* state);
        CancellationTokenRegistration Register(std::function<void()> callback, bool useSynchronizationContext);

        // Operators
        bool operator==(const CancellationToken& other) const;
        bool operator!=(const CancellationToken& other) const;

        // Static properties
        static CancellationToken None();

        friend class CancellationTokenSource;
    };
}
