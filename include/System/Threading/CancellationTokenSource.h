
#pragma once

#include "System/Object.h"
#include "System/TimeSpan.h"
#include "CancellationToken.h"
#include "Timer.h"
#include <memory>

namespace System::Threading
{
    class CancellationTokenSource : public System::Object
    {
    private:
        std::shared_ptr<CancellationTokenState> state;
        std::unique_ptr<Timer> timer;

    public:
        CancellationTokenSource();
        CancellationTokenSource(int millisecondsDelay);
        CancellationTokenSource(const TimeSpan& delay);
        virtual ~CancellationTokenSource() = default;

        void Cancel();
        void Cancel(bool throwOnFirstException);
        void CancelAfter(int millisecondsDelay);
        void CancelAfter(const TimeSpan& delay);

        // Properties
        CancellationToken GetToken();
        bool GetIsCancellationRequested() const;

        // Static methods
        static CancellationTokenSource* CreateLinkedTokenSource(const CancellationToken& token);
        static CancellationTokenSource* CreateLinkedTokenSource(const CancellationToken& token1, const CancellationToken& token2);
        static CancellationTokenSource* CreateLinkedTokenSource(const std::vector<CancellationToken>& tokens);
    };
}
