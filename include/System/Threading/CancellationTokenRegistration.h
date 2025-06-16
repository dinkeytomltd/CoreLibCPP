
#pragma once

#include "System/Object.h"
#include <memory>
#include <atomic>

namespace System::Threading
{
    struct CancellationTokenState;
    struct CancellationToken;

    struct CancellationTokenRegistration
    {
    private:
        std::weak_ptr<CancellationTokenState> tokenState;
        int registrationId;
        static std::atomic<int> nextRegistrationId;

    public:
        CancellationTokenRegistration();
        CancellationTokenRegistration(std::weak_ptr<CancellationTokenState> state, int id);

        void Dispose();
        bool Unregister();

        // Properties
        CancellationToken GetToken() const;

        // Operators
        bool operator==(const CancellationTokenRegistration& other) const;
        bool operator!=(const CancellationTokenRegistration& other) const;
    };
}
