
#pragma once

#include "System/Object.h"
#include "System/TimeSpan.h"
#include <shared_mutex>
#include <atomic>

namespace System::Threading
{
    enum class LockRecursionPolicy
    {
        NoRecursion = 0,
        SupportsRecursion = 1
    };

    class ReaderWriterLockSlim : public System::Object
    {
    private:
        std::shared_mutex rwMutex;
        LockRecursionPolicy recursionPolicy;
        std::atomic<int> readersWaiting{0};
        std::atomic<int> writersWaiting{0};
        std::atomic<int> currentReadCount{0};
        thread_local static int recursiveReadCount;
        thread_local static int recursiveWriteCount;
        thread_local static int recursiveUpgradeCount;

    public:
        ReaderWriterLockSlim();
        ReaderWriterLockSlim(LockRecursionPolicy recursionPolicy);
        virtual ~ReaderWriterLockSlim() = default;

        // Reader lock
        void EnterReadLock();
        bool TryEnterReadLock(int millisecondsTimeout);
        bool TryEnterReadLock(const TimeSpan& timeout);
        void ExitReadLock();

        // Writer lock
        void EnterWriteLock();
        bool TryEnterWriteLock(int millisecondsTimeout);
        bool TryEnterWriteLock(const TimeSpan& timeout);
        void ExitWriteLock();

        // Upgradeable read lock
        void EnterUpgradeableReadLock();
        bool TryEnterUpgradeableReadLock(int millisecondsTimeout);
        bool TryEnterUpgradeableReadLock(const TimeSpan& timeout);
        void ExitUpgradeableReadLock();

        // Properties
        bool GetIsReadLockHeld() const;
        bool GetIsWriteLockHeld() const;
        bool GetIsUpgradeableReadLockHeld() const;
        int GetCurrentReadCount() const;
        int GetRecursiveReadCount() const;
        int GetRecursiveWriteCount() const;
        int GetRecursiveUpgradeCount() const;
        int GetWaitingReadCount() const;
        int GetWaitingWriteCount() const;
        int GetWaitingUpgradeCount() const;
        LockRecursionPolicy GetRecursionPolicy() const;
    };
}
